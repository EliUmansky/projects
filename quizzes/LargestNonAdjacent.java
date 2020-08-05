
public class LargestNonAdjacent {

	public static int findSum(int[] arr, int index) {
		if (index >= arr.length) {
			return 0;
		}
		
		return Math.max(arr[index] + findSum(arr, index + 2), findSum(arr, index + 1));
	}
	
	public static void main(String[] args) {
		int[] arr = {2, 4, 6, 2, 5};
		int[] arr2 = {5, 1, 1, 5};
		System.out.println(findSum(arr, 0));
		System.out.println(findSum(arr2, 0));

	}

}
