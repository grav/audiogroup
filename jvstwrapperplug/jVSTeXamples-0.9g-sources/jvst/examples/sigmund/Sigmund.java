package jvst.examples.sigmund;

import jvst.examples.jaydlay.JayDLay;
import jvst.wrapper.VSTPluginAdapter;
import java.lang.Math;

public class Sigmund extends VSTPluginAdapter {
	public final static int PARAM_ID_TYPE = 0;
	public final static int PARAM_ID_DISTORT = 1;
	public final static int PARAM_ID_Q = 2;
	public final static int NUM_PARAMS = 3;

	private enum Dist {
		SIGMOID {
			float eval(float in, float fDist, float none){
				float dist = (float)Math.pow(10, fDist*2);

				return	 (float)((1-Math.exp(-dist*in))/(1+Math.exp(-dist*in)));
			}
			@Override
			public String toString(){
				return "Sigmoid";
			}

		},

		TUBE {
			float eval(float in, float fDist,float fQ){
				float dist  = 99 * fDist + 1; 
				float q = fQ-1;
				double out = (in - q) / (1 - Math.exp(-dist * (in - q)));
				if(out==Double.NaN){
					out=0;
				}
				out += q / (1 - Math.exp(dist * q));
				if(out==Double.NaN){
					out=0;
				}
				return (float)out;
			}

			@Override
			public String toString(){
				return "Tube";
			}

		},

		FUZZ {
			float eval(float in, float fDist, float none){
				float dist = (float)Math.pow(10, fDist*2);
				double s = dist * Math.signum(in);
				return (float) (s*(1-Math.exp(-s * in))/dist);
			}
			@Override
			public String toString(){
				return "FuzzExp";
			}

		},

		SYMCLIP {

			@Override
			float eval(float in, float b, float c) {
				// TODO Auto-generated method stub
				float out;
				if (Math.abs(in) >= 0 && Math.abs(in) < 1/3){
					out = 2 * in;
				} else if (Math.abs(in) >= 1/3 && Math.abs(in) < 2/3){
					out = (float) (Math.signum(in) * (3-Math.pow((2-3 * Math.abs(in)),2) / 3));
				} else {
					out = 1 * Math.signum(in);
				}
				return out;
			}
			@Override
			public String toString(){
				return "SymClip";
			}



		};




		abstract float eval(float a, float b, float c);

		// range per type
		private static float range = 1f/Dist.values().length;

		public static Dist get(float v){
			int index = (int)Math.floor(v/range);
			index = Math.min(Dist.values().length-1,index);
			return Dist.values()[index];
		}

		public static float getValue(Dist d){
			// TODO 
			return 0f;
		}
	}

	float fDist = 0;
	float fTargetDist = 0;

	float fQ = 0;

	Dist distortion = Dist.SIGMOID;

	public Sigmund(long Wrapper) {
		super(Wrapper);
		this.setNumInputs(1);// mono input
		this.setNumOutputs(1);// mono output
		//this.hasVu(false); //deprecated as of vst2.4
		this.canProcessReplacing(true);//mandatory for vst 2.4!
		this.setUniqueID('s'<<24 | 'g'<<16 | 'm'<<8 | 'd');//random unique number registered at steinberg (4 byte)

		this.canMono(true); 
		log("constructor for sigmund invoked");

	}

	public int canDo(String feature) {
		//the host asks us here what we are able to do
		int ret = JayDLay.CANDO_NO;

		//if (feature.equals(JayDLay.CANDO_PLUG_1_IN_2_OUT)) ret = JayDLay.CANDO_YES;
		if (feature.equals(JayDLay.CANDO_PLUG_1_IN_1_OUT)) ret = JayDLay.CANDO_YES;
		if (feature.equals(JayDLay.CANDO_PLUG_PLUG_AS_CHANNEL_INSERT)) ret = JayDLay.CANDO_YES;
		if (feature.equals(JayDLay.CANDO_PLUG_PLUG_AS_SEND)) ret = JayDLay.CANDO_YES;

		log("canDo: " + feature + " = " + ret);
		return ret;
	}

	public int getPlugCategory() {
		log("getPlugCategory");
		return VSTPluginAdapter.PLUG_CATEG_EFFECT;
	}

	public String getProductString() {
		return "Sigmund 1.0";
	}

	public String getProgramNameIndexed(int arg0, int arg1) {
		// TODO Auto-generated method stub
		return "";
	}

	public String getVendorString() {
		// TODO Auto-generated method stub
		return "Betafunk";
	}

	public boolean setBypass(boolean arg0) {
		log("setBypass");
		//do not support soft bypass!
		return false;
	}

	public boolean string2Parameter(int arg0, String arg1) {
		log("string2parameter");
		// TODO Auto-generated method stub
		return false;
	}

	public int getNumParams() {
		log("getNumParams");
		return Sigmund.NUM_PARAMS;
	}

	public int getNumPrograms() {
		log("getNumPrograms");
		return 0;
	}

	public float getParameter(int index) {
		switch(index){
		case Sigmund.PARAM_ID_TYPE: return Dist.getValue(this.distortion);
		case Sigmund.PARAM_ID_DISTORT: return this.fTargetDist;
		}
		return 0;
	}

	public String getParameterDisplay(int index) {
		return "";
	}

	public String getParameterLabel(int index) {
		switch(index){
		case Sigmund.PARAM_ID_TYPE: 
			return distortion.toString();
		case Sigmund.PARAM_ID_DISTORT: return ""+this.fTargetDist;
		case Sigmund.PARAM_ID_Q: return "";
		}
		return "";
	}

	public String getParameterName(int index) {
		switch(index){
		case Sigmund.PARAM_ID_TYPE: return "Type";
		case Sigmund.PARAM_ID_DISTORT: return "Distortion";
		case Sigmund.PARAM_ID_Q: return "Q";
		}
		return "";
	}

	public int getProgram() {
		// TODO Auto-generated method stub
		return 0;
	}

	public String getProgramName() {
		// TODO Auto-generated method stub
		return "";
	}

	public void processReplacing(float[][] ins, float[][] outs, int sampleFrames) {
		float[] in1 = ins[0];
		float[] out1 = outs[0];
		for(int i=0; i<sampleFrames;i++){

			// Scaling
			float delta = fTargetDist-fDist;
			if(Math.abs(delta)<0.05	) {
				fDist = fTargetDist;
			} else {
				fDist += Math.signum(delta) * 0.05;
			}

			out1[i] = distortion.eval(in1[i], fDist, fQ);
		}
	}

	public void setParameter(int index, float value) {
		switch(index){
		case Sigmund.PARAM_ID_TYPE: 
			distortion = Dist.get(value);
			break;
		case Sigmund.PARAM_ID_DISTORT: 
			this.fTargetDist=value; 
			break;
		case Sigmund.PARAM_ID_Q:
			this.fQ = value;
			break;
		}
	}

	public void setProgram(int arg0) {
		// TODO Auto-generated method stub

	}

	public void setProgramName(String arg0) {
		// TODO Auto-generated method stub

	}

	public static void main(String[] args){
		int n = 4;
		float r = 1f/n;
		System.out.println(r);
		float i = 0;
		while(i<=1){
			i+=0.1;
			System.out.println(i+" -> " + Math.floor(i / r));
		}
	}

}
