import java.util.ArrayList;
import java.util.List;

public class FindMissing {

	public static List<Integer> findMissing(int[] arr) {
		List<Integer> list = new ArrayList<>();
		
		for (int i = 0; i < arr.length; ++i) {
			while (arr[i] != i + 1 && arr[i] > 0) {
				if (arr[i] != arr[arr[i] - 1]) {
					swap(arr, i, arr[i] - 1);		
				} else {
					arr[i] = 0;
				}					
			}
		}
		
		for (int i = 0; i < arr.length; ++i) {
			if (arr[i] == 0) {
				list.add(i + 1);
			}
		}
		return list;
	}

	public static void swap(int[] arr, int ind1, int ind2) {
		int temp = arr[ind1];
		arr[ind1] = arr[ind2];
		arr[ind2] = temp;
		
	}
	public static void main(String[] args) {
		int[] arr = {2, 1, 16, 14, 15, 2, 5, 1, 7, 8, 9, 10, 11, 12, 13, 1, 15, 6, 5};
		int[] arr2 = {4, 3, 2, 7, 8, 2, 3, 1};
		int[] arr3 = {2, 3, 4, 5, 5};

		System.out.println(findMissing(arr));
		System.out.println(findMissing(arr2));
		System.out.println(findMissing(arr3));
	}

}
