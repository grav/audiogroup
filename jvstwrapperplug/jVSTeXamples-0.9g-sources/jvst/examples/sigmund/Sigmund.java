package jvst.examples.sigmund;

import jvst.examples.jaydlay.JayDLay;
import jvst.wrapper.VSTPluginAdapter;
import java.lang.Math;

public class Sigmund extends VSTPluginAdapter {
	public final static int PARAM_ID_DISTORT = 0;
	public final static int NUM_PARAMS = 1;
	
	float fDist = 0;
	float fTargetDist = 0;
	
	public Sigmund(long Wrapper) {
		super(Wrapper);
	    this.setNumInputs(1);// mono input
	    this.setNumOutputs(1);// mono output
	    //this.hasVu(false); //deprecated as of vst2.4
	    this.canProcessReplacing(true);//mandatory for vst 2.4!
	    this.setUniqueID('j'<<24 | 'D'<<16 | 'l'<<8 | 'y');//random unique number registered at steinberg (4 byte)

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
		case Sigmund.PARAM_ID_DISTORT: return this.fDist;
		}
		return 0;
	}

	public String getParameterDisplay(int index) {
		switch(index){
		case Sigmund.PARAM_ID_DISTORT: return "Distortion";
		}
		return "";
	}

	public String getParameterLabel(int index) {
		switch(index){
		case Sigmund.PARAM_ID_DISTORT: return ""+this.fDist;
		}
		return "";
	}

	public String getParameterName(int index) {
		return getParameterDisplay(index);
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
			int c = (int)Math.pow(10, fDist*2);
			out1[i]=(float)((1-Math.exp(-c*in1[i]))/(1+Math.exp(-c*in1[i])));
			float delta = fTargetDist-fDist;

			// TODO Scaling
			if(Math.abs(delta)<0.05	) {
				fDist = fTargetDist;
			} else {
				fDist += Math.signum(delta) * 0.05;
			}
		}
	}

	public void setParameter(int index, float value) {
		switch(index){
		case Sigmund.PARAM_ID_DISTORT: this.fTargetDist=value; break;
		}
	}

	public void setProgram(int arg0) {
		// TODO Auto-generated method stub

	}

	public void setProgramName(String arg0) {
		// TODO Auto-generated method stub

	}

}
