import java.util.Comparator;
import java.util.PriorityQueue;

public class OverlapInterval {
	
	class Element {
		private String startOrEnd;
		private int time;
		
		public Element(String startOrEnd, int time) {
			this.startOrEnd = startOrEnd;
			this.time = time;
		}
		
		public String getStartOrEnd() {
			return startOrEnd;
		}
		public Integer getTime() {
			return time;
		}
	}
	
	public static int maxOverlapIntervalCount(int[] start, int[] end) {
		OverlapInterval oi = new OverlapInterval();
		Comparator<Element> comp = (Element e1, Element e2) -> e1.getTime().compareTo(e2.getTime());
		PriorityQueue<Element> queue = new PriorityQueue<>(comp);
		int tempMaxInterval = 0;
		int maxInterval = 0;
		
		for (int i = 0; i < start.length; ++i) {
			queue.add(oi.new Element("start", start[i]));
			queue.add(oi.new Element("end", end[i]));
		}
		
		while (!queue.isEmpty()) {
			if (queue.remove().getStartOrEnd().equals("start")) {
				++tempMaxInterval;
			} else {
				maxInterval = Math.max(maxInterval, tempMaxInterval);
				--tempMaxInterval;
			}
		}
		
		return maxInterval;
	}
	
	
	public static void main(String[] args) {
		int[] start = {0, 3, 4, 7, 1};
		int[] end = {2, 7, 6, 8, 5};
		int[] start2 = {0, 9, 2, 1, 15, 7, 11, 7};
		int[] end2 = {20, 10, 3, 10, 18, 13, 16, 10};
		int[] start3 = {1, 2};
		int[] end3 = {3, 4};
		System.out.println(maxOverlapIntervalCount(start, end));
		System.out.println(maxOverlapIntervalCount(start2, end2));
		System.out.println(maxOverlapIntervalCount(start3, end3));

	}

}
