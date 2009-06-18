package jvst.betafunk;

import jvst.betafunk.phasepaint.PhasePaint;

public abstract class Parameter{
	public String label;
	public float targetValue=0;
	protected float value=0;
	private float inc=0.05f;
	public float computedValue;

	public Parameter(String label){
		this.label=label;
		this.compute();
	}

	public void slope(){
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

	protected abstract void compute();

}
