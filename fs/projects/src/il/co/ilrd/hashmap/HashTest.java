package il.co.ilrd.hashmap;

import static org.junit.jupiter.api.Assertions.*;

import java.util.Collection;
import java.util.Iterator;
import java.util.Set;
import java.util.Map.Entry;

import org.junit.jupiter.api.Test;

import il.co.ilrd.pair.Pair;

class HashTest {

	@SuppressWarnings("unlikely-arg-type")
	@Test
	void CreatePutRemoveSizeIsEmpty() {
		HashMap<Integer, String> hash = new HashMap<Integer, String>(10); 
		Pair<Integer, String> pair1 = Pair.of(1, "A");
		Pair<Integer, String> pair2 = Pair.of(2, "B");
		Pair<Integer, String> pair3 = Pair.of(3, "C");
		Pair<Integer, String> pair4 = Pair.of(4, "D");
		Pair<Integer, String> pair5 = Pair.of(5, "E");
		Pair<Integer, String> pair6 = Pair.of(6, "F");
		Pair<Integer, String> pair7 = Pair.of(7, "G");
		Pair<Integer, String> pair8 = Pair.of(8, "H");
		Pair<Integer, String> pair9 = Pair.of(null, "I");
		
		assertEquals(0, hash.size());
		assertEquals(true, hash.isEmpty());
		
		hash.put(pair1.getKey(), pair1.getValue());
		hash.put(pair2.getKey(), pair2.getValue());
		hash.put(pair3.getKey(), pair3.getValue());
		hash.put(pair4.getKey(), pair4.getValue());
		hash.put(pair5.getKey(), pair5.getValue());
		hash.put(pair6.getKey(), pair6.getValue());
		hash.put(pair7.getKey(), pair7.getValue());
		hash.put(pair8.getKey(), pair8.getValue());
		hash.put(pair9.getKey(), pair9.getValue());
		
		assertEquals(9, hash.size());
		assertEquals(false, hash.isEmpty());
		
		hash.remove(pair2.getKey());
		assertEquals(8, hash.size());
		hash.remove(9);
		assertEquals(8, hash.size());
		hash.remove(pair3.getValue());
		assertEquals("C", pair3.getValue());
		assertEquals(8, hash.size());
		hash.remove(null);
		assertEquals(7, hash.size());
		
		hash.put(666, "The number of the beast");
		String bla = hash.remove(666);
		assertEquals(bla, "The number of the beast");
	}
	
	@Test
	void PutAll() {
		HashMap<Integer, String> hash1 = new HashMap<Integer, String>(10); 
		HashMap<Integer, String> hash2 = new HashMap<Integer, String>(10); 
		
		Pair<Integer, String> pair1 = Pair.of(1, "A");
		Pair<Integer, String> pair2 = Pair.of(2, "B");
		Pair<Integer, String> pair3 = Pair.of(3, "C");
		Pair<Integer, String> pair4 = Pair.of(4, "D");
		Pair<Integer, String> pair5 = Pair.of(5, "E");
		Pair<Integer, String> pair6 = Pair.of(6, "F");
		Pair<Integer, String> pair7 = Pair.of(7, "G");
		Pair<Integer, String> pair8 = Pair.of(8, "H");
		
		hash1.put(pair1.getKey(), pair1.getValue());
		hash1.put(pair2.getKey(), pair2.getValue());
		hash1.put(pair3.getKey(), pair3.getValue());
		hash1.put(pair4.getKey(), pair4.getValue());
		hash2.put(pair5.getKey(), pair5.getValue());
		hash2.put(pair6.getKey(), pair6.getValue());
		hash2.put(pair7.getKey(), pair7.getValue());
		hash2.put(pair8.getKey(), pair8.getValue());
		
		hash1.putAll(hash2);
		
		assertEquals(8, hash1.size());
		
	}
	
	@Test
	void GetContainsKeyAndValueClear() {
		HashMap<Integer, String> hash1 = new HashMap<Integer, String>(10); 
		HashMap<Integer, String> hash2 = new HashMap<Integer, String>(10); 
		
		Pair<Integer, String> pair1 = Pair.of(1, "A");
		Pair<Integer, String> pair2 = Pair.of(2, "B");
		Pair<Integer, String> pair3 = Pair.of(3, "C");
		Pair<Integer, String> pair4 = Pair.of(4, "D");
		Pair<Integer, String> pair5 = Pair.of(5, "E");
		Pair<Integer, String> pair6 = Pair.of(6, "F");
		Pair<Integer, String> pair7 = Pair.of(7, "G");
		Pair<Integer, String> pair8 = Pair.of(8, "H");
		Pair<Integer, String> pair9 = Pair.of(9, null);
		Pair<Integer, String> pair10 = Pair.of(null, "I");
		
		hash1.put(pair1.getKey(), pair1.getValue());
		hash1.put(pair2.getKey(), pair2.getValue());
		hash1.put(pair3.getKey(), pair3.getValue());
		hash1.put(pair4.getKey(), pair4.getValue());
		hash2.put(pair5.getKey(), pair5.getValue());
		hash2.put(pair6.getKey(), pair6.getValue());
		hash2.put(pair7.getKey(), pair7.getValue());
		hash2.put(pair8.getKey(), pair8.getValue());
		hash2.put(pair9.getKey(), pair9.getValue());
		hash2.put(pair10.getKey(), pair10.getValue());
		
		assertEquals("C", hash1.get(3));
		assertNull(hash2.get(3));
		
		assertTrue(hash1.containsKey(3));
		assertFalse(hash1.containsKey(6));
		assertTrue(hash2.containsKey(null));
		assertFalse(hash1.containsKey(null));
		
		assertTrue(hash1.containsValue("B"));
		assertFalse(hash1.containsValue("H"));
		assertTrue(hash2.containsValue(null));
		assertFalse(hash1.containsValue(null));
		
		hash1.clear();

		assertTrue(hash1.isEmpty());
		
	}
	
	@Test
	void EntrySetEtc() {
		HashMap<Integer, String> hash = new HashMap<Integer, String>(10); 
		Pair<Integer, String> pair1 = Pair.of(1, "A");
		Pair<Integer, String> pair2 = Pair.of(2, "B");
		Pair<Integer, String> pair3 = Pair.of(3, "C");
		Pair<Integer, String> pair4 = Pair.of(4, "D");
		Pair<Integer, String> pair5 = Pair.of(5, "E");
		Pair<Integer, String> pair6 = Pair.of(6, "F");
		Pair<Integer, String> pair7 = Pair.of(7, "G");
		Pair<Integer, String> pair8 = Pair.of(8, "H");
		Pair<Integer, String> pair9 = Pair.of(null, "I");
		
		
		hash.put(pair1.getKey(), pair1.getValue());
		hash.put(pair2.getKey(), pair2.getValue());
		hash.put(pair3.getKey(), pair3.getValue());
		hash.put(pair4.getKey(), pair4.getValue());
		hash.put(pair5.getKey(), pair5.getValue());
		hash.put(pair6.getKey(), pair6.getValue());
		hash.put(pair7.getKey(), pair7.getValue());
		hash.put(pair8.getKey(), pair8.getValue());
		hash.put(pair9.getKey(), pair9.getValue());
		
		Set<Entry<Integer, String>> entrySet = hash.entrySet();
		Iterator<Entry<Integer, String>> entryIter = entrySet.iterator();

		int size = 0;
		
		while (entryIter.hasNext()) {
			++size;
			entryIter.next();
		}
		
		assertEquals(size, hash.size());
		
		Set<Integer> keySet = hash.keySet();
		Iterator<Integer> keyIter = keySet.iterator();
		
		size = 0;
		
		while (keyIter.hasNext()) {
			++size;
			keyIter.next();
		}
		
		assertEquals(size, hash.size());
	
		Collection<String> values = hash.values();
		Iterator<String> valueIter = values.iterator();
		
		size = 0;
		
		while (valueIter.hasNext()) {
			++size;
			valueIter.next();
		}

		assertEquals(size, hash.size());
	}
	
	@Test
	void EntrySetEtcRun() {
		HashMap<Integer, String> hash = new HashMap<Integer, String>(10); 
		Pair<Integer, String> pair1 = Pair.of(1, "A");
		Pair<Integer, String> pair2 = Pair.of(2, "B");
		Pair<Integer, String> pair3 = Pair.of(3, "C");
		Pair<Integer, String> pair4 = Pair.of(4, "D");
		Pair<Integer, String> pair5 = Pair.of(5, "E");
		Pair<Integer, String> pair6 = Pair.of(6, "F");
		Pair<Integer, String> pair7 = Pair.of(7, "G");
		Pair<Integer, String> pair8 = Pair.of(8, "H");
		Pair<Integer, String> pair9 = Pair.of(null, "I");
		
		
		hash.put(pair1.getKey(), pair1.getValue());
		hash.put(pair2.getKey(), pair2.getValue());
		hash.put(pair3.getKey(), pair3.getValue());
		hash.put(pair4.getKey(), pair4.getValue());
		hash.put(pair5.getKey(), pair5.getValue());
		hash.put(pair6.getKey(), pair6.getValue());
		hash.put(pair7.getKey(), pair7.getValue());
		hash.put(pair8.getKey(), pair8.getValue());
		hash.put(pair9.getKey(), pair9.getValue());
		
		Set<Entry<Integer, String>> entrySet = hash.entrySet();
		Iterator<Entry<Integer, String>> entryIter = entrySet.iterator();

		Set<Integer> keySet = hash.keySet();
		Iterator<Integer> keyIter = keySet.iterator();

		Collection<String> values = hash.values();
		Iterator<String> valueIter = values.iterator();
		
		while (entryIter.hasNext()) {
			System.out.println(entryIter.next());			
		}
		System.out.println(" ");
		
		while (keyIter.hasNext()) {
			System.out.print(keyIter.next() + " ");
		}
		System.out.println(" ");
		
		while (valueIter.hasNext()) {
			System.out.print(valueIter.next() + " ");
		}
		System.out.println(" ");
		
		hash.put(3, "T");
		Collection<String> values2 = hash.values();
		Iterator<String> valueIter2 = values2.iterator();
		
		while (valueIter2.hasNext()) {
			System.out.print(valueIter2.next() + " ");
		}
	}
}
