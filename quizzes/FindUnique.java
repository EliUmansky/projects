
public class FindUnique {

	public static int findUnique(int[] arr) {
		int unique = 0;

		for (int i = 0; i < arr.length; ++i) {
			unique ^= arr[i];
		}
		
		return unique;
	}
	
	public static void main(String[] args) {
		int[] arr = {1, 2, 3, 4, 1, 2, 3};
		int[] arr2 = {1, 2, 1, 2};
		int[] arr3 = {1};
		System.out.println(findUnique(arr));
		System.out.println(findUnique(arr2));
		System.out.println(findUnique(arr3));
	}

}
