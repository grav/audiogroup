package jvst.betafunk.phasepaint;


import java.lang.Math;
import java.util.Arrays;

import jvst.betafunk.Parameter;

public class PhasePaint extends EasyAdapter {

	int bufferSize = 3; 

	int lfoCounter = 0;

	static float fs = 44100;

	Parameter filterFreq = new FilterFreq();
	Parameter sweepFreq = new SweepFreq();
	Parameter wetness = new Parameter("Wetness"){
		@Override
		protected void compute() {
			computedValue=value;
		}

	};
	
	Parameter[] allParameters = {sweepFreq,filterFreq,wetness};
	Parameter[] slopeParameters = allParameters;



	float[] x = new float[3];
	float[] y = new float[3];
	int n=2;  // last index in x & y

	//	private float[] fcs = {300, 1200, 3000, 6000, 10000, 12000};
//	private float[] fcs = {3000f};


	public class FilterFreq extends Parameter{

		FilterFreq() {
			super("Filter Frequency");
		}

		@Override
		protected void compute() {
			computedValue=value*9700+300;
		}

	}

	public class SweepFreq extends Parameter{
		SweepFreq(){
			super("Sweep Frequency");
		}

		@Override
		protected void compute(){
			computedValue=value*9.9f+0.1f;
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

		//		inBuffer = new MyQueue<Float>(bufferSize,0f);
		//		outBuffer = new MyQueue<Float>(bufferSize,0f);

		log("constructor for phasepaint invoked");

	}

	private static void allpass2ndorder(float fc, float fb,float[] b, float[] a){
		// TODO: dont hardwire 

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

		float[] xInit = x.clone(); float[] yInit = y.clone(); 

		//		for (float fc:fcs ){
		x=xInit.clone(); y=yInit.clone();
		for(int i=0; i<sampleFrames;i++){
			lfoCounter+=1;

			slope();


			x[0]=x[1]; x[1]=x[2]; x[2]=in[i];
			y[0]=y[1]; y[1]=y[2]; y[2]=0;

			// calculate filter parameters
			float fc = filterFreq.computedValue;
			
			float fb = fc/70;

			float fcLFO = (float) (fc + fc/2 * Math.sin(2 * Math.PI * lfoCounter* sweepFreq.computedValue/fs));

			// calculate filter coefficients
			float[] b = {0,0,0};
			float[] a = {0,0,0};
			allpass2ndorder(fcLFO,fb,b,a);

			float bsum = 0;
			for (int j=0; j < b.length; j++){
				bsum += b[j] * x[n-j];
			}

			float asum = 0;
			for (int j=1; j < a.length; j++){
				asum += a[j] * y[n-j];
			}
			out[i] += 1/a[0] * (bsum - asum);
			y[2]=out[i];
			out[i] += in[i] * (1-wetness.computedValue);

		}

		//		}
	}

	public String getProductString() {
		return "PhasePaint 1.0";
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

		int k = Integer.MAX_VALUE-2;
		System.out.println(k);
		k+=1;
		System.out.println(k);
	}

}
