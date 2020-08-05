
public class MaxArea {

	public static int maxArea(int[] height) {
		if (height.length < 2) {
			return 0;
		}
		
		int max = 0;
		int tempMax = 0;
		int left = 0;
		int right = height.length - 1;

		while (left < right) {
			tempMax = findArea(height, left, right);
			max = Math.max(tempMax, max);
			
			if (height[left] > height[right]) {
				--right;
			} else {
				 ++left;
			}
		}
		
		return max;
	}
	
	private static int findArea(int[] height, int firstIndex, int secondIndex) {
		return (secondIndex - firstIndex) * Math.min(height[firstIndex], height[secondIndex]);
	}
	
	public static void main(String[] args) {
		int[] arr = {1,8,6,2,5,4,8,3,7};
		System.out.println(maxArea(arr));
	}

}
