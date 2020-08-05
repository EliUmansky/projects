import java.util.LinkedList;
import java.util.List;

public class SumTreeRootToLeaf {

	static class Node {
		Integer value;
		Node left, right;
		
		Node(Integer item) {
			value = item;
			left = right = null;
		}
	}
	
	public static int findSum(Node root) {
		List<Integer> list = new LinkedList<>();
		recFindSum(root, list, "0");
		int sum = 0;
		
		for (Integer num : list) {
			sum += num;
		}
		
		return sum;
	}
	
	private static void recFindSum(Node curr, List<Integer> list, String sum) {
		if (null == curr) { return; }
		
		sum += curr.value.toString();
		if (isLeaf(curr)) {
			list.add(Integer.parseInt(sum));
			return;
		}
		
		recFindSum(curr.left, list, sum);
		recFindSum(curr.right, list, sum);
	}
	
	private static boolean isLeaf(Node curr) {
		return curr.left == null && curr.right == null;
	}
	public static void main(String[] args) {
		Node root1 = new Node(1);
		Node b = new Node(2);
		Node c = new Node(3);
		
		root1.left = b;
		root1.right = c;
		
		
		Node root2 = new Node(4);
		Node d = new Node(9);
		Node e = new Node(0);
		root2.left = d;
		root2.right = e;
		d.left = new Node(5);
		d.right = new Node(1);
		
		System.out.println(findSum(root1));
		System.out.println(findSum(root2));
		
	}

}
