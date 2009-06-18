package jvst.betafunk;

// this class implements a look-back buffer which allows us to go BACK IN TIME!
public class LookBackBuffer {

	private float[] lookBack;
	
	private float[] buffer;
	
	public LookBackBuffer(int size){
		lookBack = new float[size];
		buffer = new float[size];
	}
	
	public void setBuffer(float[] newBuffer){
		assert(newBuffer.length>=lookBack.length);
		// take last n of old buffer and place in lookback
		for(int i=0;i<lookBack.length; i++){	
			lookBack[i]=buffer[buffer.length-lookBack.length+i];
		}
		buffer = newBuffer;
	}
	
	public int getLookBackLength(){
		return lookBack.length;
	}
	
	public float[] getBuffer(){
		return buffer;
	}
	
	public float get(int index){
		if(index<0){
			return lookBack[lookBack.length+index];
		} else {
			return buffer[index];
		}
	}
	
	public void put(int index,float f){
		buffer[index]=f;
	}
	
	public void flush(int newSize){
		setBuffer(new float[newSize]);
	}
	
	public void flush(){
		flush(buffer.length);
	}
	
	public static void main(String[] args){
		// tests
		LookBackBuffer lb = new LookBackBuffer(3);
		float[] buffer = {1f,2f,3f,4f};
		lb.setBuffer(buffer);
		assert(lb.get(-1)==0f);
		assert(lb.get(2)==3f);
		lb.flush();
		assert(lb.get(-1)==4f);
		assert(lb.get(-2)==3f);
		assert(lb.get(-3)==2f);
		float[] newBuffer = {5f,6f,7f,8f};
		lb.setBuffer(newBuffer);
		assert(lb.get(0)==5f);
		assert(lb.get(-1)==0f);
	}
		
}
