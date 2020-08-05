package il.co.ilrd.generic_singly_linked_list;

import java.util.ConcurrentModificationException;
import java.util.Iterator;

public class SinglyLinkedList<T> implements Iterable<T> {

	private Node<T> head = new Node<T>(null, null);
	private int version = 0;
	
	private static class Node<T> {
		
		private T data;
		private Node<T> nextNode;

		private Node(T data, Node<T> nextNode) {
			this.data = data;
			this.nextNode = nextNode;
		}
		public T getData() {
			return data;
		}
		public Node<T> getNextNode() {
			return nextNode;
		}
	}
	
	private static class ListIteratorImp<T> implements Iterator<T> {

		private Node<T> curr;
		private int iterVersion;
		SinglyLinkedList<T> list;
		
		private ListIteratorImp(SinglyLinkedList<T> list) {
			curr = list.head;
			this.list = list;
			iterVersion = list.getVersion();
		}
		@Override
		public boolean hasNext() {
			if (iterVersion != list.getVersion()) {
				throw new ConcurrentModificationException("ConcurrentModificationException");
			}
			
			return (null != curr.getNextNode());
		}
		
		@Override
		public T next() {
			if (iterVersion != list.getVersion()) {
				throw new ConcurrentModificationException("ConcurrentModificationException");
			}
			
			T dataHolder = (T)curr.getData();
			curr = curr.getNextNode();
			
			return dataHolder;
		}
	}
	
	@Override
	public ListIteratorImp<T> iterator() {
		return new ListIteratorImp<T>(this);
	}
	public int getVersion() {
		return version;
	}
	public void pushFront(T data) {		
		if (null == data) { throw new WrongDataException("WrongDataException"); }
		
		Node<T> newNode = new Node<>(data, head);
		head = newNode;
		++version;
	}
	public T popFront() throws EmptyListException {
		if (isEmpty()) { throw new EmptyListException("EmptyListException"); }
		
		T data = head.getData();
		head = head.getNextNode();
		++version;
		
		return data;
	}
	public int size() {
		ListIteratorImp<T> iter = iterator();	
		int count = 0;

		while (iter.hasNext()) {
			++count;
			iter.next();
		}
		
		return count;
	}
	public Iterator<T> find(T data) {
		if (isEmpty()) { return null; }

		ListIteratorImp<T> iter = iterator();
		
		while (iter.hasNext() && !(iter.curr.getData().equals(data))) {
			iter.next();
		}

		return (iter.hasNext()) ? iter : null;
	}
	public boolean isEmpty() {
		return (null == head.getNextNode());
	}
	public static <E> SinglyLinkedList<E> Merge(SinglyLinkedList<E> list1, SinglyLinkedList<E> list2) {
		SinglyLinkedList<E> newList = new SinglyLinkedList<>();
        
		for(E element : SinglyLinkedList.newReverse(list1)) {
            newList.pushFront(element);
        }
        for(E element: SinglyLinkedList.newReverse(list2)) {
            newList.pushFront(element);
        }
        
        return newList;
	}
	public static <E> SinglyLinkedList<E> newReverse(SinglyLinkedList<E> list) {
		SinglyLinkedList<E> newList = new SinglyLinkedList<>();
		
		for(E element : list) {
            newList.pushFront(element);
        }
		
		return newList;
	}
	public static <E> void Print(SinglyLinkedList<E> list) {
		 for (E element: list) {System.out.print(element + " ");}
	     System.out.println();
	}
}

class EmptyListException extends RuntimeException {  
	private static final long serialVersionUID = 1L;

	EmptyListException(String s){  
        super(s);  
    }  
}

class WrongDataException extends RuntimeException {  
	private static final long serialVersionUID = 1L;

	WrongDataException(String s){  
        super(s);  
    }  
}