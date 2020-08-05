import java.util.HashMap;
import java.util.Map;

public class PalStr {

	public static boolean canPermute2Palindrome(String str) {
		Map<Character, Character> map = new HashMap<>();
		
		for (int i = 0; i < str.length(); ++i) {
			if (map.containsKey(str.charAt(i))) {
				map.remove(str.charAt(i));
			} else {
				map.put(str.charAt(i), str.charAt(i));
			}
		}
		
		return map.size() < 2;
	}
	public static void main(String[] args) {
		System.out.println(canPermute2Palindrome("code"));
		System.out.println(canPermute2Palindrome("aab"));
		System.out.println(canPermute2Palindrome("carerac"));

	}

}
