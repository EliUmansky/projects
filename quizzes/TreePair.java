import java.util.HashMap;
import java.util.Map;

public class TreePair {
	
	static class Node {
		int value;
		Node left, right;
		
		Node(int item) {
			value = item;
			left = right = null;
		}
	}
	
	public static void findPair(Node root, int sum) {
		Map<Integer, Node> map = new HashMap<>();
		recFind(root, sum, map);
	}
	
	private static void recFind(Node curr, int sum, Map<Integer, Node> map) {
		if (null == curr) { return; }
		
		if (map.containsKey(curr.value)) {
			System.out.println(curr.value + " + " + map.get(curr.value).value + " = " + sum);
			return;
		}
		
		map.put(sum - curr.value, curr);
		recFind(curr.left, sum, map);
		recFind(curr.right, sum, map);
	}
	
	public static void main(String[] args) {
		Node root = new Node(20);
		Node b = new Node(10);
		Node c = new Node(5);
		
		root.left = b;
		root.right = c;
		b.left = new Node(12);
		b.right = new Node(2);
		c.left = new Node(8);
		c.right = new Node(25);
		
		findPair(root, 35);
		findPair(root, 7);
		findPair(root, 12);
		findPair(root, 10);
	}
	
}
