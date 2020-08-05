
public class Matrix {

	public static int matrix(int[][] arr, int n) {
		int count = 0;
		
		for (int i = n - 1; i >= 0; --i) {
			if (arr[i][i] != 1) { return count; }
			if (areAllZeros(arr, n, i, i)) { ++count; }
		}
		return count;
	}
	
	private static boolean areAllZeros(int[][] arr, int n, int x, int y) {
		for (int i = x + 1; i < n; ++i) {
			if (arr[i][y] != 0) { return false; }
		}
		for (int i = y + 1; i < n; ++i) {
			if (arr[x][i] != 0) { return false; }
		}
		return true;
	}
	
	public static void main(String[] args) {
		int[][] arr1 = {{2, 3, 4, 5}, {6, 7, 8, 9}, {2, 3, 4, 0}, {3, 3, 3, 1}};
		int[][] arr2 = {{2, 3, 4, 5}, {3, 3, 3, 3}, {6, 7, 1, 0}, {2, 3, 0, 1}};
		int[][] arr3 = {{2, 3, 4, 5}, {3, 1, 0, 0}, {6, 0, 1, 0}, {2, 0, 0, 1}};

		System.out.println(matrix(arr1, 4));
		System.out.println(matrix(arr2, 4));
		System.out.println(matrix(arr3, 4));

	}

}
