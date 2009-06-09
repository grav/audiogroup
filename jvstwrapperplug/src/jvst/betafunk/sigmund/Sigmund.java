package jvst.betafunk.sigmund;

import jvst.wrapper.*;
import jvst.wrapper.communication.*;

import java.lang.Math;

public class Sigmund extends VSTPluginAdapter {
	public final static int PARAM_ID_TYPE = 0;
	public final static int PARAM_ID_DISTORT = 1;
	public final static int PARAM_ID_Q = 2;
	public final static int NUM_PARAMS = 3;

	public enum Dist {
		SIGMOID {
			double eval(double in, double fDist, double none){
				double dist = Math.pow(10, fDist*2);

				return	 ((1-Math.exp(-dist*in))/(1+Math.exp(-dist*in)));
			}
			@Override
			public String toString(){
				return "Sigmoid";
			}

		},

		TUBE {
			double eval(double in, double fDist,double fQ){
				double dist  = 99 * fDist + 1; 
				double q = fQ-1;
				double out = (in - q) / (1 - Math.exp(-dist * (in - q)));
				if(out==Double.NaN){
					out=0;
				}
				out += q / (1 - Math.exp(dist * q));
				if(out==Double.NaN){
					out=0;
				}
				return out;
			}

			@Override
			public String toString(){
				return "Tube";
			}

		},

		FUZZ {
			double eval(double in, double fDist, double none){
				double dist = Math.pow(10, fDist*2);
				double s = dist * Math.signum(in);
				return (s*(1-Math.exp(-s * in))/dist);
			}
			@Override
			public String toString(){
				return "FuzzExp";
			}

		},

		SYMCLIP {

			@Override
			double eval(double in, double b, double c) {
				// TODO Auto-generated method stub
				double out;
				if (Math.abs(in) >= 0 && Math.abs(in) < 1/3d){
					out = 2d * in;
				} else if (Math.abs(in) >= 1/3d && Math.abs(in) < 2/3d){
					out = Math.signum(in) * (3-Math.pow((2-3d * Math.abs(in)),2)) / 3d;
				} else {
					out = 1d * Math.signum(in);
				}
				return out;
			}
			@Override
			public String toString(){
				return "SymClip";
			}



		};

		abstract double eval(double a, double b, double c);

		// range per type
		private static double range = 1f/Dist.values().length;

		public static Dist get(double v){
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
		this.canDoubleReplacing(true);
		this.setUniqueID('s'<<24 | 'g'<<16 | 'm'<<8 | 'd');//random unique number registered at steinberg (4 byte)

		this.canMono(true); 
		log("constructor for sigmund invoked");

	}

	public int canDo(String feature) {
		//the host asks us here what we are able to do
		int ret = VSTV20ToHost.CANDO_NO;

		//if (feature.equals(JayDLay.CANDO_PLUG_1_IN_2_OUT)) ret = JayDLay.CANDO_YES;
		if (feature.equals(VSTV20ToHost.CANDO_PLUG_1_IN_1_OUT)) ret = VSTV20ToHost.CANDO_YES;
		if (feature.equals(VSTV20ToHost.CANDO_PLUG_PLUG_AS_CHANNEL_INSERT)) ret = VSTV20ToHost.CANDO_YES;
		if (feature.equals(VSTV20ToHost.CANDO_PLUG_PLUG_AS_SEND)) ret = VSTV20ToHost.CANDO_YES;

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
			double delta = fTargetDist-fDist;
			if(Math.abs(delta)<0.05	) {
				fDist = fTargetDist;
			} else {
				fDist += Math.signum(delta) * 0.05;
			}
			out1[i] = (float) (0.5 * distortion.eval(in1[i], fDist, fQ));
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
		//float[] in={-1.000000f, -0.995992f, -0.991984f, -0.987976f, -0.983968f, -0.979960f, -0.975952f, -0.971944f, -0.967936f, -0.963928f, -0.959920f, -0.955912f, -0.951904f, -0.947896f, -0.943888f, -0.939880f, -0.935872f, -0.931864f, -0.927856f, -0.923848f, -0.919840f, -0.915832f, -0.911824f, -0.907816f, -0.903808f, -0.899800f, -0.895792f, -0.891784f, -0.887776f, -0.883768f, -0.879760f, -0.875752f, -0.871743f, -0.867735f, -0.863727f, -0.859719f, -0.855711f, -0.851703f, -0.847695f, -0.843687f, -0.839679f, -0.835671f, -0.831663f, -0.827655f, -0.823647f, -0.819639f, -0.815631f, -0.811623f, -0.807615f, -0.803607f, -0.799599f, -0.795591f, -0.791583f, -0.787575f, -0.783567f, -0.779559f, -0.775551f, -0.771543f, -0.767535f, -0.763527f, -0.759519f, -0.755511f, -0.751503f, -0.747495f, -0.743487f, -0.739479f, -0.735471f, -0.731463f, -0.727455f, -0.723447f, -0.719439f, -0.715431f, -0.711423f, -0.707415f, -0.703407f, -0.699399f, -0.695391f, -0.691383f, -0.687375f, -0.683367f, -0.679359f, -0.675351f, -0.671343f, -0.667335f, -0.663327f, -0.659319f, -0.655311f, -0.651303f, -0.647295f, -0.643287f, -0.639279f, -0.635271f, -0.631263f, -0.627255f, -0.623246f, -0.619238f, -0.615230f, -0.611222f, -0.607214f, -0.603206f, -0.599198f, -0.595190f, -0.591182f, -0.587174f, -0.583166f, -0.579158f, -0.575150f, -0.571142f, -0.567134f, -0.563126f, -0.559118f, -0.555110f, -0.551102f, -0.547094f, -0.543086f, -0.539078f, -0.535070f, -0.531062f, -0.527054f, -0.523046f, -0.519038f, -0.515030f, -0.511022f, -0.507014f, -0.503006f, -0.498998f, -0.494990f, -0.490982f, -0.486974f, -0.482966f, -0.478958f, -0.474950f, -0.470942f, -0.466934f, -0.462926f, -0.458918f, -0.454910f, -0.450902f, -0.446894f, -0.442886f, -0.438878f, -0.434870f, -0.430862f, -0.426854f, -0.422846f, -0.418838f, -0.414830f, -0.410822f, -0.406814f, -0.402806f, -0.398798f, -0.394790f, -0.390782f, -0.386774f, -0.382766f, -0.378758f, -0.374749f, -0.370741f, -0.366733f, -0.362725f, -0.358717f, -0.354709f, -0.350701f, -0.346693f, -0.342685f, -0.338677f, -0.334669f, -0.330661f, -0.326653f, -0.322645f, -0.318637f, -0.314629f, -0.310621f, -0.306613f, -0.302605f, -0.298597f, -0.294589f, -0.290581f, -0.286573f, -0.282565f, -0.278557f, -0.274549f, -0.270541f, -0.266533f, -0.262525f, -0.258517f, -0.254509f, -0.250501f, -0.246493f, -0.242485f, -0.238477f, -0.234469f, -0.230461f, -0.226453f, -0.222445f, -0.218437f, -0.214429f, -0.210421f, -0.206413f, -0.202405f, -0.198397f, -0.194389f, -0.190381f, -0.186373f, -0.182365f, -0.178357f, -0.174349f, -0.170341f, -0.166333f, -0.162325f, -0.158317f, -0.154309f, -0.150301f, -0.146293f, -0.142285f, -0.138277f, -0.134269f, -0.130261f, -0.126253f, -0.122244f, -0.118236f, -0.114228f, -0.110220f, -0.106212f, -0.102204f, -0.098196f, -0.094188f, -0.090180f, -0.086172f, -0.082164f, -0.078156f, -0.074148f, -0.070140f, -0.066132f, -0.062124f, -0.058116f, -0.054108f, -0.050100f, -0.046092f, -0.042084f, -0.038076f, -0.034068f, -0.030060f, -0.026052f, -0.022044f, -0.018036f, -0.014028f, -0.010020f, -0.006012f, -0.002004f, 0.002004f, 0.006012f, 0.010020f, 0.014028f, 0.018036f, 0.022044f, 0.026052f, 0.030060f, 0.034068f, 0.038076f, 0.042084f, 0.046092f, 0.050100f, 0.054108f, 0.058116f, 0.062124f, 0.066132f, 0.070140f, 0.074148f, 0.078156f, 0.082164f, 0.086172f, 0.090180f, 0.094188f, 0.098196f, 0.102204f, 0.106212f, 0.110220f, 0.114228f, 0.118236f, 0.122244f, 0.126253f, 0.130261f, 0.134269f, 0.138277f, 0.142285f, 0.146293f, 0.150301f, 0.154309f, 0.158317f, 0.162325f, 0.166333f, 0.170341f, 0.174349f, 0.178357f, 0.182365f, 0.186373f, 0.190381f, 0.194389f, 0.198397f, 0.202405f, 0.206413f, 0.210421f, 0.214429f, 0.218437f, 0.222445f, 0.226453f, 0.230461f, 0.234469f, 0.238477f, 0.242485f, 0.246493f, 0.250501f, 0.254509f, 0.258517f, 0.262525f, 0.266533f, 0.270541f, 0.274549f, 0.278557f, 0.282565f, 0.286573f, 0.290581f, 0.294589f, 0.298597f, 0.302605f, 0.306613f, 0.310621f, 0.314629f, 0.318637f, 0.322645f, 0.326653f, 0.330661f, 0.334669f, 0.338677f, 0.342685f, 0.346693f, 0.350701f, 0.354709f, 0.358717f, 0.362725f, 0.366733f, 0.370741f, 0.374749f, 0.378758f, 0.382766f, 0.386774f, 0.390782f, 0.394790f, 0.398798f, 0.402806f, 0.406814f, 0.410822f, 0.414830f, 0.418838f, 0.422846f, 0.426854f, 0.430862f, 0.434870f, 0.438878f, 0.442886f, 0.446894f, 0.450902f, 0.454910f, 0.458918f, 0.462926f, 0.466934f, 0.470942f, 0.474950f, 0.478958f, 0.482966f, 0.486974f, 0.490982f, 0.494990f, 0.498998f, 0.503006f, 0.507014f, 0.511022f, 0.515030f, 0.519038f, 0.523046f, 0.527054f, 0.531062f, 0.535070f, 0.539078f, 0.543086f, 0.547094f, 0.551102f, 0.555110f, 0.559118f, 0.563126f, 0.567134f, 0.571142f, 0.575150f, 0.579158f, 0.583166f, 0.587174f, 0.591182f, 0.595190f, 0.599198f, 0.603206f, 0.607214f, 0.611222f, 0.615230f, 0.619238f, 0.623246f, 0.627255f, 0.631263f, 0.635271f, 0.639279f, 0.643287f, 0.647295f, 0.651303f, 0.655311f, 0.659319f, 0.663327f, 0.667335f, 0.671343f, 0.675351f, 0.679359f, 0.683367f, 0.687375f, 0.691383f, 0.695391f, 0.699399f, 0.703407f, 0.707415f, 0.711423f, 0.715431f, 0.719439f, 0.723447f, 0.727455f, 0.731463f, 0.735471f, 0.739479f, 0.743487f, 0.747495f, 0.751503f, 0.755511f, 0.759519f, 0.763527f, 0.767535f, 0.771543f, 0.775551f, 0.779559f, 0.783567f, 0.787575f, 0.791583f, 0.795591f, 0.799599f, 0.803607f, 0.807615f, 0.811623f, 0.815631f, 0.819639f, 0.823647f, 0.827655f, 0.831663f, 0.835671f, 0.839679f, 0.843687f, 0.847695f, 0.851703f, 0.855711f, 0.859719f, 0.863727f, 0.867735f, 0.871743f, 0.875752f, 0.879760f, 0.883768f, 0.887776f, 0.891784f, 0.895792f, 0.899800f, 0.903808f, 0.907816f, 0.911824f, 0.915832f, 0.919840f, 0.923848f, 0.927856f, 0.931864f, 0.935872f, 0.939880f, 0.943888f, 0.947896f, 0.951904f, 0.955912f, 0.959920f, 0.963928f, 0.967936f, 0.971944f, 0.975952f, 0.979960f, 0.983968f, 0.987976f, 0.991984f, 0.995992f, 1.000000f};

		//		System.out.print("[");
		//		for(float f: in){
		//			float g = (float) Dist.SYMCLIP.eval(f, 0, 0);
		//			System.out.print(g+",");
		//		}
		//		System.out.println("]");

		//		int n = 4;
		//		float r = 1f/n;
		//		System.out.println(r);
		//		float i = 0;
		//		while(i<=1){
		//			i+=0.1;
		//			System.out.println(i+" -> " + Math.floor(i / r));
		//		}
	}

}
