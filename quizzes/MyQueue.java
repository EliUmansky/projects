import java.util.Stack;

public class MyQueue {
	Stack<Integer> left;
	Stack<Integer> right;
	
	public MyQueue() {
		left = new Stack<>();
		right = new Stack<>();
	}
	
	public void push(int x) {
		right.add(x);
	}
	
	public Integer pop() {
		if (left.isEmpty()) {
			moveAllElements();
		} 
		
		return left.pop();
	}
	
	public Integer peek() {
		if (left.isEmpty()) {
			moveAllElements();
		}
		
		return left.peek();
	}
	
	public boolean isEmpty() {
		return left.isEmpty();
	}
	
	private void moveAllElements() {
		while (!right.isEmpty()) {
			left.push(right.pop());
		}
	}
}
