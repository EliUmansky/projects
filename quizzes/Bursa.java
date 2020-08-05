
public class Bursa {
	
	public static void bursa(int[] arr) {
		int buyIndex = 0;
		int sellIndex = 0;
		int totalProfit = 0;
		int tempBuyIndex = 0;
		int tempProfit = 0;
		
		for (int i = 0; i < arr.length; ++i) {
			tempProfit = arr[i] - arr[tempBuyIndex];
			if (tempProfit > totalProfit) {
				buyIndex = tempBuyIndex;
				sellIndex = i;
				totalProfit = tempProfit;
			} else if (tempProfit <= 0) {
				tempProfit = 0;
				tempBuyIndex = i;
			}
			
		}
		
		System.out.println("buy index: " + buyIndex + " sell index: " + sellIndex + " profit: " + totalProfit);;
	}
	public static void main(String[] args) {
		int[] arr = {6, 12, 3, 5, 1, 4, 9, 2};
		bursa(arr);
	}
}
