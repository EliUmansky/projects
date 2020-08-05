import java.util.HashMap;

public class FindPair {

	public static void findPair(int[] arr, int sum) {
		HashMap<Integer, Integer> map = new HashMap<>();
		
		for (int i = 0; i < arr.length; ++i) {
			Integer match = map.get(arr[i]);
			
			if (null != match) {
				System.out.println(arr[i] + " + " + match + " = " + sum);
				break;
			}
			
			map.put(sum - arr[i], arr[i]);
		}
	}
	public static void main(String[] args) {
		int[] arr = {1, 2, 3, 4, 5};
		findPair(arr, 5);

	}

}
