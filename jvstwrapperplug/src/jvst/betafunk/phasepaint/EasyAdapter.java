package jvst.betafunk.phasepaint;

import jvst.betafunk.Parameter;
import jvst.wrapper.VSTPluginAdapter;
import jvst.wrapper.communication.VSTV20ToHost;

public abstract class EasyAdapter extends VSTPluginAdapter {

	protected Parameter[] allParameters;
	protected Parameter[] slopeParameters;
	
	public EasyAdapter(long Wrapper) {
		super(Wrapper);
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
		return allParameters.length;
	}

	public int getNumPrograms() {
		log("getNumPrograms");
		return 0;
	}

	public float getParameter(int index) {
		return allParameters[index].targetValue;
	}

	public String getParameterDisplay(int index) {
		return "";
	}

	public String getParameterLabel(int index) {
		return "";
	}

	public String getParameterName(int index) {
		return allParameters[index].label;
	}

	public int getProgram() {
		// TODO Auto-generated method stub
		return 0;
	}

	public String getProgramName() {
		// TODO Auto-generated method stub
		return "";
	}

	public void setParameter(int index, float value) {
		allParameters[index].targetValue=value;
	}

	public void setProgram(int arg0) {
		// TODO Auto-generated method stub
	
	}

	public void setProgramName(String arg0) {
		// TODO Auto-generated method stub
	
	}


	public void slope() {
		// Move towards target values (slope)
		for (Parameter p : slopeParameters){
			p.slope();
		}
	}

}