
public class CalcTree {

	private static Operations[] arr = new Operations[48];
	
	static {
		arr[43] = new Add();
		arr[45] = new Sub();
		arr[42] = new Mul();
		arr[47] = new Div();
	}
	
	static class Node {
		char value;
		Node left, right;
		
		Node(char item) {
			value = item;
			left = right = null;
		}
	}
	
	public static double calc(Node root) {
		if (null == root.left && null == root.right) {
			return Character.getNumericValue(root.value);
		}
		return arr[root.value].operate(calc(root.left), calc(root.right));
	}

	public static void main(String[] args) {
		Node a = new Node('*');
		Node b = new Node('+');
		Node c = new Node('+');
		
		a.left = b;
		a.right = c;
		b.left = new Node('3');
		b.right = new Node('2');
		c.left = new Node('4');
		c.right = new Node('5');

		System.out.println(calc(a));
	}

}
