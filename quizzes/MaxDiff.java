
public class MaxDiff {

	public static int maxDiff(int[] arr) {
		int tempMax = 0;
		int maxProfit = 0;
		int buyInd = 0;
		
		for (int i = 0; i < arr.length; ++i) {
			tempMax = arr[i] - arr[buyInd];
			if (tempMax < 0) { buyInd = i; }
			
			maxProfit = Math.max(maxProfit, tempMax);
		}
		
		return maxProfit;
	}

	public static void main(String[] args) {
		int[] arr = {1, 2, 3, 4, 5, 6, 7, 8, 9};
		System.out.println(maxDiff(arr));
		int[] arr2 = {5, 3, 4, 1, 6, 7, 2, 4};
		System.out.println(maxDiff(arr2));
		int[] arr3 = {3, -4, -1, -6, -7, -2, -4};
		System.out.println(maxDiff(arr3));
	}

}
