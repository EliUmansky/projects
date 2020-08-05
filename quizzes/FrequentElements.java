import java.util.PriorityQueue;

public class FrequentElements {

	public static void freq(PriorityQueue<Integer> queue, int k) {
		Integer[] dequeueArr = new Integer[queue.size()];
		Integer[] kIndicator = new Integer[k];
		dequeueArr[0] = queue.remove();
		kIndicator[0] = dequeueArr[0];
		int index = 0;
		
		for (int j = 0; j != k - 1 && !queue.isEmpty(); ++index) {
			dequeueArr[index] = queue.remove();
			if (dequeueArr[index] != kIndicator[j]) {
				++j;
				kIndicator[j] = dequeueArr[index];
			}
		}
		
		returnElementsToQueue(queue, dequeueArr, index - 1);
		print(kIndicator);
	}
	
	private static void returnElementsToQueue(PriorityQueue<Integer> queue, Integer[] arr, int index) {
		while (index >=0) {
			queue.add(arr[index]);
			--index;
		}		
	}

	private static void print(Integer[] arr) {
		for (int i = 0; i < arr.length; ++i) {
			System.out.print(arr[i] + " ");
		}
		System.out.println();
		
	}

	public static void main(String[] args) {
		PriorityQueue<Integer> queue = new PriorityQueue<>();

		queue.add(1);
		queue.add(2);
		queue.add(1);
		queue.add(3);
		queue.add(1);
		queue.add(2);

		freq(queue, 2);
		freq(queue, 3);
	}

}
