
public class MaxConsElements {

	public static int findMax(int[] arr, int k) {
		int sum = 0;
		int index = 0;
		
		if (k > arr.length) {
			System.out.println("invalid input");
			return -1;
		}
		
		for (; index < k; ++index) {
			sum += arr[index];
		}
		int maxSum = sum;
		int lower = 0;
		
		for (;index < arr.length; ++index, ++lower) {
			sum = sum - arr[lower] + arr[index];
			maxSum = Math.max(sum, maxSum);
		}

		return maxSum;
	}
	public static void main(String[] args) {
		int[] arr = {100, 200, 300, 400};
		int[] arr2 = {1, 4, 2, 10, 23, 3, 1, 0, 20};
		int[] arr3 = {2, 3};
		int[] arr4 = {1, 2, 3, 4, 5, 6, -10, 100};
		
		System.out.println(findMax(arr, 2));
		System.out.println(findMax(arr2, 4));
		System.out.println(findMax(arr4, 3));
		System.out.println(findMax(arr3, 3));
	}

}
