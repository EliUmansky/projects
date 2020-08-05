package il.co.ilrd.dict_sort;

public class Threads implements Runnable{
	int histogram[];
	char[] arr;
	int startIndex;
	int endIndex;
	int[] localHistogram = new int[256];
	
	public Threads(int histogram[], char[] arr, int startIndex, int endIndex) {
		
		this.histogram = histogram;
		this.arr = arr;
		this.startIndex = startIndex;
		this.endIndex = endIndex;
	}
	@Override
	public void run() {
		BufferSort.Sort(histogram, arr, startIndex, endIndex, localHistogram);
	}

}


