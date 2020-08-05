
public class Division {
	static int divide(int dividend, int divisor) {
		if (0 == divisor) {
			throw new ArithmeticException();
		}
		
		String posOrNeg = null;
		int result = 0;
		
		if (dividend < 0 && divisor < 0) {
			dividend = ~(dividend - 1);
			divisor = ~(divisor - 1);
		} else if (dividend < 0) {
			posOrNeg = "-";
			dividend = ~(dividend - 1);
		} else if (divisor < 0) {
			posOrNeg = "-";
			divisor = ~(divisor - 1);
		}
		while (dividend >= divisor) {
			++result;
			dividend -= divisor;
		}
		
		return Integer.parseInt(posOrNeg += result);
	}
	
	public static void main(String[] args) {
		System.out.println(Division.divide(43, -8));
	}

}
