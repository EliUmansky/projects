
public class MinPathSum {
	class TreeNode {
		int value;
		TreeNode right;
		TreeNode left;
	}
	
	public static int minPathSum(TreeNode root) {
		if (null == root) { return 0; }
		
		if (null == root.left) { return root.value + minPathSum(root.right); }
		else if (null == root.right) { return root.value + minPathSum(root.left); }
		else { return root.value + Math.min(minPathSum(root.left), minPathSum(root.right));	}
	}
}
