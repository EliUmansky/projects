
public class UniquePath {

	public static int findUniquePaths(int rows, int columns) {
		if (columns == 0 || rows == 0 ) { return 0; }
		if (1 == columns && 1 == rows) { return 1; }

		return findUniquePaths(rows - 1, columns) + findUniquePaths(rows, columns - 1);
	}
	public static void main(String[] args) {
		System.out.println(findUniquePaths(7, 7));
	}

}
