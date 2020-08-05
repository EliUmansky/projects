import java.util.ArrayList;

public class IntegerWeight {

	ArrayList<Object> list;

	public IntegerWeight() {
		list = new ArrayList<>();
		init();
	}
	
	public void init() {
		ArrayList<Object> inner = new ArrayList<>();
		ArrayList<Object> innerInner = new ArrayList<>();
		list.add(1);
		innerInner.add(6);
		inner.add(4);
		inner.add(innerInner);
		list.add(inner);
	}
	
	public int getSumOfWeights() {
		return recGetSumOfWeights(list, 1);
	}
	
	private int recGetSumOfWeights(ArrayList<Object> list, int depth) {
		int sum = 0;
		for (int i = 0; i < list.size(); ++i) {
			if (list.get(i) instanceof Integer) {
				sum += depth * (Integer) list.get(i);
			} else if (list.get(i) instanceof ArrayList){
				sum += recGetSumOfWeights((ArrayList<Object>) list.get(i), depth + 1);
			}
		}
		return sum;
	}
	
	public static void main(String[] args) {
		IntegerWeight iw = new IntegerWeight();
		System.out.println(iw.getSumOfWeights());
	}

}
