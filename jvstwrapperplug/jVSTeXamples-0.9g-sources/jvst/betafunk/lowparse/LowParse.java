package jvst.betafunk.lowparse;

import jvst.betafunk.MyQueue;
import jvst.examples.jaydlay.JayDLay;
import jvst.wrapper.VSTPluginAdapter;
import java.lang.Math;

public class LowParse extends VSTPluginAdapter {
//	public final static int NUM_PARAMS = 1;

	int bufferSize = 1; 
	public MyQueue<Float> inBuffer;
	

	public enum Params{
		// The different parameters
		LOWPASSLENGTH("Lowpass Length"){

			@Override
			void compute(LowParse pp) {
				int newSize = Math.round(value * 19 + 1);
				pp.inBuffer.setSize(newSize, 0f);
			}
		};
		
		// Constructor
		Params(String label){
			this.label=label;
		}
		
		String label;
		float targetValue;
		float value;
		static float inc = 0.05f;
		
		void slope(LowParse pp){
			float delta = targetValue-value;
			if (delta==0) return;
			if(Math.abs(delta)<inc ){
				value = targetValue;
			} else {
				value += Math.signum(delta) * inc;
			}
			compute(pp);
		}
		
		abstract void compute(LowParse pp);
		
	}
	
	// Define the parameters we'd like to slope
	private static Params[] slopeParameters = {Params.LOWPASSLENGTH};
	
	public LowParse(long Wrapper) {
		super(Wrapper);
		this.setNumInputs(1);// mono input
		this.setNumOutputs(1);// mono output
		//this.hasVu(false); //deprecated as of vst2.4
		this.canProcessReplacing(true);//mandatory for vst 2.4!
		this.setUniqueID('p'<<24 | 'h'<<16 | 'p'<<8 | 't');//random unique number registered at steinberg (4 byte)

		this.canMono(true);
		
		inBuffer = new MyQueue<Float>(bufferSize,0f);
		
		log("constructor for LowParse invoked");

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
		return "LowParse 1.0";
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
		return Params.values().length;
	}

	public int getNumPrograms() {
		log("getNumPrograms");
		return 0;
	}

	public float getParameter(int index) {
		return Params.values()[index].targetValue;
	}

	public String getParameterDisplay(int index) {
		return "";
	}

	public String getParameterLabel(int index) {
		return "";
	}

	public String getParameterName(int index) {
		return Params.values()[index].label;
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
		float[] in = ins[0];
		float[] out = outs[0];
		for(int i=0; i<sampleFrames;i++){

			// Move towards target values (slope)
			for (Params p : slopeParameters){
				p.slope(this);
			}

			inBuffer.push(in[i]);
			
			// test: simple lowpass filter
			out[i]=0;
			for(Float f: inBuffer){
				out[i] += f / inBuffer.size();
			}
		}
	}

	public void setParameter(int index, float value) {
		Params.values()[index].targetValue=value;
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
