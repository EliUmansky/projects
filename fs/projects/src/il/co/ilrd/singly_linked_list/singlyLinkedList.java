package il.co.ilrd.singly_linked_list;

public class singlyLinkedList {
	private node head = new node(null, null);
	
	public class node {
		private Object data;
		private node nextNode;
			
		public node(Object data, node nextNode) {
			this.data = data;
			this.nextNode = nextNode;
		}
		
		public Object getData() {
			return data;
		}
		
		public node getNextNode() {
			return nextNode;
		}
	}

	public class listIteratorImp implements listIterator {
		node curr = head;
		
		@Override
		public Object next() {
			Object data = curr.getData();
			
			curr = curr.getNextNode();
			return data;
		}

		@Override
		public Boolean hasNext() {
			return (null != curr.getNextNode());
		}
	}

	public Boolean isEmpty(){
		return (null == head.getNextNode());
	}
	
	public listIterator find(Object data) {
		listIterator iter = begin();
		listIterator returnIter = begin();
		
		if (isEmpty()) {
			return null;
		}
		
        while (iter.hasNext()) {
        	if (iter.next().equals(data)) { 
	            return returnIter; 
	        }
        	returnIter.next();
        }
        
        return iter.next().equals(data) ? returnIter : null; 
	}
	
	public int size() {
		listIterator iter = begin();
		int count = 1;
		
		if (isEmpty()) {
			return 0;
		}
		
		while (iter.hasNext()) {
			++count;
			iter.next();
		}
				
		return count;
	}
	
	public Object popFront() {
		Object data = head.nextNode.data;
		head.nextNode = head.nextNode.nextNode;
		
		return data;
	}
	
	public void pushFront(Object data) {
		node newNode = new node(data, head.nextNode);
		head.nextNode = newNode;
	}
	
	public listIterator begin() {
		listIterator iter = new listIteratorImp();	
		iter.next();
		
		return iter;
	}
}
