package jvst.betafunk;

import java.util.*;

public class MyQueue<T> implements Iterable<T> {

	/**
	 * 
	 */
	private static final long serialVersionUID = 888992697227010681L;

	private LinkedList<T> list;
	
	public <E extends T> MyQueue(int size, T initElement){
		list = new LinkedList<T>();
		for(int i=0; i<size;i++){
			list.add(initElement);
		}
	}
	
	// insert new element at beginning; throw away the one at the end
	public <E extends T> void push(T element){
		list.add(0, element);
		list.removeLast();
	}

	public <E extends T> T get(int index){
		return list.get(index);
	}
	
	@Override
	public String toString(){
		return list.toString();
	}
	
	@Override
	public boolean equals(Object l){
		return list.equals(l);
	}
	
	// test implementation
	public static void main(String[] args){
		System.out.print("testing...");
		MyQueue<Float> q = new MyQueue<Float>(3,0f);
		Float[] a = {0f,0f,0f};	
		q.testEquals(a);

		q.push(2f);
		Float[] b = {2f,0f,0f};
		q.testEquals(b);
		
		q.push(3f);
		Float[] c = {3f,2f,0f};
		q.testEquals(c);
		
		assert(q.get(1)==2f);
		
		System.out.println("done");
	}
	
	public int size(){
		return list.size();
	}
	
	public <E extends T> void setSize(int newSize, T element){
		while(list.size()<newSize){
			list.add(element);
		}
		while(list.size()>newSize){
			list.removeLast();
		}
	}
	
	// helper test method
	public <E extends T> void testEquals(T[] a){
		assert(this.equals(Arrays.asList(a))) : 
			Arrays.deepToString(a) + " != "+ this.toString();
	}

	public Iterator<T> iterator() {
		// TODO Auto-generated method stub
		return list.iterator();
	}
	

	
}
