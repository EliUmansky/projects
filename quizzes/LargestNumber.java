
public class LargestNumber {

	public static String largestNum(int[] arr) {
		String[] strArr = sort(arr);
		String res = "";
		
		for (int i = 0; i < strArr.length; ++i) {
			res += strArr[i];
		}
		
		return res;
	}
	
	public static String[] sort(int[] arr) {
		String[] strArr = new String[arr.length];
		
		for (int i = 0; i < arr.length; ++i) {
			strArr[i] = Integer.toString(arr[i]);
		}
		
		for (int i = 0; i < strArr.length - 1; ++i) {
			for (int j = i + 1; j < strArr.length; ++j) {
				if (isJLarger(strArr, i, j)) {
					swap(strArr, i, j);
				}
			}
		}
		
		return strArr;
	}
	
	public static boolean isJLarger(String[] strArr, int i, int j) {
		int charPos = 0;
		int maxPos = Math.min(strArr[i].length(), strArr[j].length());
		while (charPos < maxPos) {
			if (strArr[j].charAt(charPos) > strArr[i].charAt(charPos)) {
				return true;
			}
			++charPos;
		}

		if (strArr[i].length() > strArr[j].length()) {
			return (strArr[i].charAt(charPos) < strArr[j].charAt(charPos - 1));
		} else if (strArr[i].length() < strArr[j].length()) {
			return (strArr[i].charAt(charPos - 1) < strArr[j].charAt(charPos));
		} else {
			return false;			
		}
	}
	
	public static void swap(String[] strArr, int ind1, int ind2) {
		String temp = strArr[ind1];
		strArr[ind1] = strArr[ind2];
		strArr[ind2] = temp;
	}
	
	public static void main(String[] args) {
		int[] arr = {3, 30, 34, 5, 9};	
		System.out.println(largestNum(arr));
		int[] arr2 = {3334, 3332, 3, 9, 2};	
		System.out.println(largestNum(arr2));

	}

}
