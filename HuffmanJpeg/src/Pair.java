public class Pair<X,Y> { 
	private X fst;
	private Y snd; 
	public @Override int hashCode(){
		return 13*fst.hashCode()+7*snd.hashCode();
	}
	
	public Pair(X fst, Y snd) {this.fst=fst; this.snd=snd;} 
	public X getFirst() { return fst; } 
	public Y getSecond() { return snd; }
	public @Override boolean equals(Object o){
		if ( o.getClass()!=Pair.class ) return false; 
		Pair<X,Y> other = (Pair<X,Y>)o;
		return (getFirst().equals(other.getFirst()) && 
				getSecond().equals(other.getSecond()));
	}

}
