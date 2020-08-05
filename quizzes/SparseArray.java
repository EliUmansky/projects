import java.util.HashMap;

public class SparseArray {

	HashMap<Integer, Integer> map;
	int size;

	public void init(int[] arr, int reqSize) {
		this.size = reqSize;
		map = new HashMap<>(reqSize);
		for (int i = 0; i < arr.length; ++i) {
			if (0 != arr[i]) {
				map.put(i, arr[i]);
			}
		}
	}
	
	public void set(int index, int val) {
		if (index >= size) { throw new ArrayIndexOutOfBoundsException(); }
		
		if (0 == val) { return; }
		
		map.put(index, val);
	}
	
	public int get(int index) {
		if (index >= size) { throw new ArrayIndexOutOfBoundsException(); }
		
		Integer res = map.get(index);
		
		return (null == res) ? 0 : res;
	}
	
	public static void main(String[] args) {
		int[] arr = {0, 0, 0, 2, 0, 4, 0, 0, 0, 5, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0};
		SparseArray sparr = new SparseArray();
		sparr.init(arr, 3);
		System.out.println("Required 0:  " + sparr.get(2));
		System.out.println("Required 2:  " + sparr.get(3));
		System.out.println("Required 0:  " + sparr.get(4));
		System.out.println("Required 4:  " + sparr.get(5));
		System.out.println("Required 5:  " + sparr.get(9));
		System.out.println("Required 0:  " + sparr.get(10));
		System.out.println("Required 1:  " + sparr.get(18));


	}

}
