package jvst.examples.phasepaint;

import jvst.examples.jaydlay.JayDLay;
import jvst.wrapper.VSTPluginAdapter;
import java.lang.Math;

public class PhasePaint extends VSTPluginAdapter {
//	public final static int NUM_PARAMS = 1;

	
	private enum Params{
		SWEEPFREQ("SweepFreq");

		Params(String label){
			this.label=label;
		}
		
		String label;
		float targetValue;
		float value;
		static float inc = 0.05f;
		
		void slope(){
			float delta = targetValue-value;
			if(Math.abs(delta)<inc ){
				value = targetValue;
			} else {
				value += Math.signum(delta) * inc;
			}
		}
		
	}
	
	// Define the parameters we'd like to slope
	private static Params[] slopeParameters = {Params.SWEEPFREQ};
	
	public PhasePaint(long Wrapper) {
		super(Wrapper);
		this.setNumInputs(1);// mono input
		this.setNumOutputs(1);// mono output
		//this.hasVu(false); //deprecated as of vst2.4
		this.canProcessReplacing(true);//mandatory for vst 2.4!
		this.setUniqueID('p'<<24 | 'h'<<16 | 'p'<<8 | 't');//random unique number registered at steinberg (4 byte)

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
		return "PhasePaint 1.0";
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
		float[] in1 = ins[0];
		float[] out1 = outs[0];
		for(int i=0; i<sampleFrames;i++){

			// Move towards target values (slope)
			for (Params p : slopeParameters){
				p.slope();
			}
			
			// Scaling
//			float delta = fTargetDist-fDist;
//			if(Math.abs(delta)<0.05	) {
//				fDist = fTargetDist;
//			} else {
//				fDist += Math.signum(delta) * 0.05;
//			}
//
			out1[i] = in1[i];
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
