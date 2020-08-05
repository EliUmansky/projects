package il.co.ilrd.pair;

import static org.junit.jupiter.api.Assertions.*;

import java.util.Comparator;

import org.junit.jupiter.api.Test;

class PairTest {

	@Test
	void basicTest() {
		Pair<Integer, String> pair1 = Pair.of(0, "f");
		Pair<Integer, String> pair2 = Pair.of(2, "b");
		Pair<Integer, String> pair3 = Pair.of(8, "c");
		Pair<Integer, String> pair4 = Pair.of(2, "d");
		Pair<Integer, String> pair5 = Pair.of(null, null);
		Pair<String, Integer> pair6 = Pair.swap(pair3);
		Pair<String, Integer> pair7 = Pair.of("c", 8);
		Integer[] arr = {2, 5, 6, 1, 3, 8, 4};
		String[] arr2 = {"c", "f", "b", "a", "d"};
		Pair<Integer, Integer> pair8 = Pair.minMax(arr);
		Pair<String, String> pair9 = Pair.minMax(arr2);
		
		assertEquals(0, pair1.setKey(1));
		assertEquals("f", pair1.setValue("a"));		
		assertEquals(1, pair1.getKey());
		assertNull(pair5.getValue());
		
		pair4.setValue("b");
		assertEquals(pair2, pair4);
		assertEquals(pair6, pair7);
		assertEquals(pair5, Pair.swap(pair5));
		
		assertFalse(pair7.equals(pair8));
		assertTrue(pair2.equals(pair4));
		
		assertEquals(1, pair8.getKey());
		assertEquals(8, pair8.getValue());
		
		assertEquals("a", pair9.getKey());
		assertEquals("f", pair9.getValue());
		
		
		Pair<Integer, Integer> pair10 = Pair.minMax(arr, new Comparator<Integer>() {
		    public int compare(Integer num1, Integer num2) {
		        return num1.compareTo(num2);
		    }
		});
		
		assertEquals(pair8, pair10);
		
		
		Comparator<String> comp = new Comparator<String>() {
		    public int compare(String num1, String num2) {
		        return num1.compareTo(num2);
		    }
		};
		
		Pair<String, String> pair11 = Pair.minMax(arr2, comp);	
		assertEquals(pair9, pair11);
		
		Comparator<String> comp2 = (String num1, String num2) -> {return num1.compareTo(num2);};
		
		Pair<String, String> pair12 = Pair.minMax(arr2, comp2);
		assertEquals(pair9, pair12);
	}
}
