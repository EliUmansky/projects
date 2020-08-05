
public class SwapPairs {

	class Node {
		int data;
		Node next;
		
		public Node(int data, Node next) {
			this.data = data;
			this.next = next;
		}
	}

	public Node swapPairs(Node head) {
		if (null == head) { return null; }
		if (null == head.next) { return head; }
		
		return recSwap(head, head.next);
	}
	private Node recSwap(Node head, Node next) {
		if (null == next || null == next.next) { return head; }

		head.next = recSwap(next.next, next.next.next);
		next.next = head;
		
		return next;
	}
	public static void main(String[] args) {
		SwapPairs sp = new SwapPairs();
		Node dummy = sp.new Node(666, null);
		Node four = sp.new Node(4, dummy);
		Node three = sp.new Node(3, four);
		Node two = sp.new Node(2, three);
		Node one = sp.new Node(1, two);

		Node iter = sp.swapPairs(one);
		while (null != iter.next) {
			System.out.println(iter.data);
			iter = iter.next;
		}
	}

}
