import java.util.LinkedList;
import java.util.Queue;
import java.util.Random;

public class Shuffle {
	int[] arr;
	Queue<Integer> queue = new LinkedList<>();
	
	public Shuffle(int[] initialArr, int m) {
		this.arr = new int[initialArr.length - m];
		int i = 0;
		
		for (; i < arr.length; ++i) {
			arr[i] = initialArr[i];
		}
		
		for (; i < initialArr.length; ++i) {
			queue.add(initialArr[i]);
		}
	}
	
	public int getRandom() {
		int randIndex = new Random().nextInt(arr.length);
		
		int temp = arr[randIndex];
		arr[randIndex] = queue.remove();
		queue.add(temp);

		return temp;
	}
}
