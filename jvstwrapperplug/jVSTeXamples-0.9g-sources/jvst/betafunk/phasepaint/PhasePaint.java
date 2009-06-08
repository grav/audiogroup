package jvst.betafunk.phasepaint;

import jvst.betafunk.MyQueue;
import jvst.examples.jaydlay.JayDLay;
import jvst.wrapper.VSTPluginAdapter;
import java.lang.Math;
import java.util.Arrays;

public class PhasePaint extends VSTPluginAdapter {

	int bufferSize = 3; 
	int zeroCrossings = 0;

	Parameter filterFreq = new FilterFreq();
	
	// Define the parameters we'd like to slope
	private Parameter[] allParameters = {filterFreq};
	private Parameter[] slopeParameters = {filterFreq};

	public MyQueue<Float> inBuffer,outBuffer;

	public abstract class Parameter{
		String label;
		float targetValue=0;
		float value=0;
		float inc=0.05f;
		float computedValue;
		
		Parameter(String label){
			this.label=label;
			this.compute();
		}
				
		void slope(PhasePaint pp){
			float delta = targetValue-value;
			if (delta==0) return;
			if(Math.abs(delta)<inc ){
				value = targetValue;
			} else {
				value += Math.signum(delta) * inc;
			}
			compute();
		}

		float getCompValue(){
			return this.computedValue;
		}
		
		abstract void compute();

	}

	public class FilterFreq extends Parameter{

		FilterFreq() {
			super("Filter Frequency");
		}

		@Override
		void compute() {
			computedValue=value*9700+300;
		}
		
	}
	

	public PhasePaint(long Wrapper) {
		super(Wrapper);
		this.setNumInputs(1);// mono input
		this.setNumOutputs(1);// mono output
		//this.hasVu(false); //deprecated as of vst2.4
		this.canProcessReplacing(true);//mandatory for vst 2.4!
		this.setUniqueID('p'<<24 | 'h'<<16 | 'p'<<8 | 't');//random unique number registered at steinberg (4 byte)

		this.canMono(true);

		inBuffer = new MyQueue<Float>(bufferSize,0f);
		outBuffer = new MyQueue<Float>(bufferSize,0f);

		log("constructor for phasepaint invoked");

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

	private static void allpass2ndorder(float fc, float fb,float[] b, float[] a){
		// TODO: dont hardwire 
		float fs = 44100;

		//	DAFX eq. 2.19 - 2.20
		float c = (float) ((Math.tan(Math.PI*fb/fs) - 1)  / (Math.tan(Math.PI*fb/fs) + 1));
		float d = (float) -Math.cos(2 * Math.PI * fc/fs);

		// DAFX eq. 2.21
		b[0] = -c; b[1]= d*(1-c); b[2] = 1;
		a[0] = -1; a[1]= -d*(1-c); a[2] = c;

	}

	public void processReplacing(float[][] ins, float[][] outs, int sampleFrames) {
		float[] in = ins[0];
		float[] out = outs[0];
		for(int i=0; i<sampleFrames;i++){

			// Move towards target values (slope)
			for (Parameter p : slopeParameters){
				p.slope(this);
			}

			inBuffer.push(in[i]);

//			for (float fc:fcs){
				// calculate filter parameters
				//fc = fc + fc * sin(2 * pi * n * lfo/fs);

				float fc = filterFreq.computedValue;
				float fb = fc/70;

				// calculate filter coefficients
				float[] b = {0,0,0};
				float[] a = {0,0,0};
				allpass2ndorder(fc,fb,b,a);

				float bsum = 0;
				for (int j=0; j < b.length; j++){
					bsum += b[j] * inBuffer.get(j);
				}

				float asum = 0;
				for (int j=1; j < a.length; j++){
					asum += a[j] * outBuffer.get(j-1);
				}
				out[i] += 1/a[0] * (bsum - asum);
				//				out[i]=in[i];

//			}
			//			out[i]=(out[i]+in[i])/2;
			outBuffer.push(out[i]);

		}
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

	public static void main(String[] args){
		int n = 4;
		float r = 1f/n;
		System.out.println(r);
		float i = 0;
		while(i<=1){
			i+=0.1;
			System.out.println(i+" -> " + Math.floor(i / r));
		}
		float[] b = {0,0,0};
		float[] a = {0,0,0};

		allpass2ndorder(3000f,3000f/70f,a,b);
		System.out.println(Arrays.toString(a));
		System.out.println(Arrays.toString(b));
	}

}
