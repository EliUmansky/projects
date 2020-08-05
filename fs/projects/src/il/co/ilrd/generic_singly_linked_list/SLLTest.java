package il.co.ilrd.generic_singly_linked_list;

import static org.junit.jupiter.api.Assertions.*;

import java.util.ArrayList;
import java.util.Iterator;
import java.util.List;

import org.junit.jupiter.api.Test;

class SLLTest {

	@Test
	void testInteger() {
		SinglyLinkedList<Integer> list1 = new SinglyLinkedList<Integer>();
		
		Iterator<Integer> resultIter;
		
		assertTrue(list1.isEmpty());
		assertEquals(0, list1.size());
		
		list1.pushFront(6);
		list1.pushFront(5);
		list1.pushFront(4);
		list1.pushFront(3);
		list1.pushFront(2);
		list1.pushFront(1);
		
		assertFalse(list1.isEmpty());
		assertEquals(6, list1.size());
		
		list1.popFront();
		list1.popFront();
		
		assertEquals(4, list1.size());
		
		resultIter = list1.find(5);
		assertEquals(5, resultIter.next());
		resultIter = list1.find(8);
		assertEquals(null, resultIter);
		
		list1 = SinglyLinkedList.newReverse(list1);
		
		list1.popFront();
		list1.popFront();
		list1.popFront();
		
		resultIter = list1.find(6);
		assertNull(resultIter);
		resultIter = list1.find(3);
		assertEquals(3, resultIter.next());
		
		list1.pushFront(666);
		
		try {
			resultIter.next();
        } catch(Exception e) {
            e.getMessage();
            assertEquals(e.getMessage(), "ConcurrentModificationException");
        }	
		
		try {
			list1.pushFront(null);
        } catch(Exception e) {
            e.getMessage();
            assertEquals(e.getMessage(), "WrongDataException");
        }

		for (int i = list1.size(); i > 0; --i) {
			list1.popFront();
		}
		
		try {
			list1.popFront();
        } catch(Exception e) {
            e.getMessage();
            assertEquals(e.getMessage(), "EmptyListException");
        }
	}

	@Test
	void testMerge() {
		SinglyLinkedList<Integer> list1 = new SinglyLinkedList<Integer>();
		SinglyLinkedList<Integer> list2 = new SinglyLinkedList<Integer>();
		SinglyLinkedList<Integer> list3;
		
		assertTrue(list1.isEmpty());
		assertEquals(0, list1.size());
		
		list1.pushFront(6);
		list1.pushFront(5);
		list1.pushFront(4);
		list2.pushFront(3);
		list2.pushFront(2);
		list2.pushFront(1);
	
		list3 = SinglyLinkedList.Merge(list1, list2);
		
		assertEquals(6, list3.size());
	}
	
	@Test
	void testIntegerList() {
		SinglyLinkedList<List<Integer>> sll = new SinglyLinkedList<List<Integer>>();
		Iterator<List<Integer>> iter;
		
		List<Integer> list1 = new ArrayList<Integer>();
		List<Integer> list2 = new ArrayList<Integer>();
		List<Integer> list3 = new ArrayList<Integer>();
		
		list1.add(1);
		list1.add(2);
		list1.add(3);
		
		list2.add(4);
		list2.add(5);
		list2.add(6);
		
		list3.add(7);
		list3.add(8);
		list3.add(9);
		
		sll.pushFront(list3);
		sll.pushFront(list2);
		sll.pushFront(list1);
		
		SinglyLinkedList.Print(sll);
		
		iter = sll.find(list2);
		assertEquals(list2, iter.next());
	}
}

