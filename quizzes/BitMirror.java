
public class BitMirror {

	public static int reverse(int num) {
		int res = 0;
		int counter = 0;
		while (num != 0) {
			res |= num & 1;
			res <<= 1;
			num >>= 1;
			++counter;
		}
		return res << (31 - counter); 
	}

	public static void main(String[] args) {
		System.out.println(reverse(0));
		System.out.println(reverse(3));
		System.out.println(reverse(127));

	}

}
