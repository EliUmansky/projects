import java.util.Comparator;
import java.util.PriorityQueue;
import java.util.Scanner;

public class Median {

	public void coundMedian() {
		Scanner scanner = new Scanner(System.in);
		String input = null;
		Integer[] arr = null;
		Integer element = null;
		Comparator<Integer> comp = new Comparator<Integer>() {
			@Override
			public int compare(Integer e1, Integer e2) {
				int res = e1.compareTo(e2);
				
				return res;
			}
		};
		PriorityQueue<Integer> queue = new PriorityQueue<>();
		while (true) {
			System.out.print("Type some integer: ");
			input = scanner.nextLine();
			if (input.equalsIgnoreCase("stop")) {
				break;
			}
			element = Integer.parseInt(input);
			
			queue.add(element);
			arr = new Integer[queue.size()];
			arr = queue.toArray(arr);
			
			for (Integer i : queue) {
				System.out.print(i);
			}
			System.out.println("");
			if (queue.size() % 2 == 0) {
				System.out.println(((float)(arr[arr.length / 2] + (float)arr[arr.length / 2 - 1]) / 2));
			} else {
				System.out.println(arr[arr.length / 2]);
			}
		}
		int i = 0;
		while ( i < queue.size()) {
			System.out.println(queue.peek());
			queue.remove();
		}
		scanner.close();
	}
	public static void main(String[] args) {
		Median med = new Median();
		med.coundMedian();
		
	}

}
