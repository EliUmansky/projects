import java.util.Comparator;
import java.util.PriorityQueue;


public class HeapStack<T> {
	
	Comparator<Element> comp =
			(Element e1, Element e2)->e2.timestamp.compareTo(e1.timestamp);
	PriorityQueue<Element> heap = new PriorityQueue<>(comp);

	class Element {
		private T value;
		private Long timestamp;
		
		public Element(T value) {
			this.value = value;
			this.timestamp = System.nanoTime();
		}
	}
	
	public void push(T value) {
		heap.add(new Element(value));
	}
	
	public T pop() {
		return heap.remove().value;
	}
	
	public static void main(String[] args) throws InterruptedException {
		HeapStack<Integer> stack = new HeapStack<>();
		stack.push(3);
		stack.push(2);
		stack.push(4);
		stack.push(1);
		stack.push(5);
		stack.push(0);

		for (int i = 0; i < 6; ++i) {
			System.out.println(stack.pop());			
		}
		
		HeapStack<String> stack2 = new HeapStack<>();
		stack2.push("dear?");
		stack2.push("my ");
		stack2.push("on, ");
		stack2.push("going ");
		stack2.push("what's ");
		stack2.push("Hello, ");
		
		for (int i = 0; i < 6; ++i) {
			System.out.print(stack2.pop());			
		}
	}
}
