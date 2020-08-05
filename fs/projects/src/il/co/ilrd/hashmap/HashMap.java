package il.co.ilrd.hashmap;
import java.util.AbstractCollection;
import java.util.AbstractSet;
import java.util.ArrayList;
import java.util.Collection;
import java.util.ConcurrentModificationException;
import java.util.Iterator;
import java.util.LinkedList;
import java.util.List;
import java.util.Map;
import java.util.Set;
import il.co.ilrd.pair.Pair;

public class HashMap<K,V> implements Map<K, V>{

	private List<List<Pair<K, V>>> hashMap;
	private final int capacity;
	private final static int DEFAULT_CAPACITY = 16; 
	protected int mode = 0;
	
	public HashMap() {
		this(DEFAULT_CAPACITY);
	};
	
	public HashMap(int capacity) {
		this.capacity = capacity;
		hashMap = new ArrayList<List<Pair<K, V>>>(capacity);
	
		for(int i = 0; i < capacity; ++i) {
			hashMap.add(new LinkedList<Pair<K, V>>());
		}
	}
	
	@Override
	public void clear() {
		++mode;
		for (List<Pair<K, V>> bucket : hashMap) {
			bucket.clear();
		}
	}
	
	@Override
	public boolean containsKey(Object key) {
		return (null != getPair(key));
	}

	@Override
	public boolean containsValue(Object value) {
		for (List<Pair<K, V>> bucket : hashMap) {
			for (Pair<K, V> pair : bucket) {
				try {
					if (pair.getValue().equals(value)) { return true; }
				} catch (NullPointerException e) {
					if (pair.getValue() == value) { return true; }
				}
			}
		}
		
		return false;
	}

	@Override
	public Set<Entry<K, V>> entrySet() {
		return new EntrySet();
	}

	@Override
	public V get(Object key) {
		Pair<K, V> pair = getPair(key);
		
		return (null == pair) ? null : pair.getValue();
	}

	@Override
	public boolean isEmpty() {
		Iterator<Entry<K, V>> iter = entrySet().iterator();
		
		return !iter.hasNext();
	}

	@Override
	public Set<K> keySet() {
		return new KeySet();
	}

	private int getIndex(Object key) {
		if (null == key) { return 0; }
		else { return key.hashCode() % capacity; }
	}
	
	@Override
	public V put(K key, V value) {
		int index = getIndex(key);
		Pair<K, V> pair = getPair(key);
		V oldValue = null;
		
		++mode;
		if (null != pair) { oldValue = pair.setValue(value); }
		else { hashMap.get(index).add(Pair.of(key, value)); }
		
		return oldValue;
	}

	private Pair<K, V> getPair(Object key) {
		int index = getIndex(key);
		
		for (Pair<K, V> pair : hashMap.get(index) ) {
			if (keyEquals(key, pair.getKey())) { return pair; }
		}
		
		return null;
	}
	private boolean keyEquals(Object key1, Object key2 ) {
		try {
			if (key1.equals(key2)) { return true; }
		} catch (NullPointerException e) {
			if (key1 == key2) { return true; }
		}
		
		return false;
	}
	@Override
	public void putAll(Map<? extends K, ? extends V> map) {		
		++mode;
		for (Entry<? extends K, ? extends V> entry : map.entrySet()) {
			put(entry.getKey(), entry.getValue());
		}
	}

	@Override
	public V remove(Object key) {
		int index = getIndex(key);
		int listIndex = 0;
		
		for (Pair<K, V> pair : hashMap.get(index)) {
			if (keyEquals(key, pair.getKey())) {
				return hashMap.get(index).remove(listIndex).getValue();
			}	
			++listIndex;
		}
		
		return null;		
	}

	@Override
	public int size() {
		int size = 0;
		
		for (List<Pair<K, V>> bucket : hashMap) {
			size += bucket.size();
		}
		
		return size;
	}

	@Override
	public Collection<V> values() {
		return new ValueCollection();
	}
	
	private class EntrySet extends AbstractSet<Map.Entry<K, V>>{
		
		private int entrySetMode = mode;
		
		@Override
		public Iterator<Entry<K, V>> iterator() {
			return new EntryIterator();
		}

		@Override
		public int size() {
			return HashMap.this.size();
		}
		
		private class EntryIterator implements Iterator<Map.Entry<K, V>>{

			private Iterator<List<Pair<K, V>>> indexIter = hashMap.iterator();
			private Iterator<Pair<K, V>> listIter = indexIter.next().iterator();
			
			@Override
			public boolean hasNext() {
				while (indexIter.hasNext() && !listIter.hasNext()) {
					listIter = indexIter.next().iterator();
				}	
				
				return listIter.hasNext();
			}

			@Override
			public Entry<K, V> next() {
				if (entrySetMode != mode) {
					throw new ConcurrentModificationException();
				}
				
				Entry<K, V> entry = null;
				
				if (hasNext()) {
					entry = listIter.next();					
				} 
				
				return entry;
			}	
		}
	}
	
	private class KeySet extends AbstractSet<K> implements Iterator<K> {
		
		private Iterator<Map.Entry<K, V>> iter = new EntrySet().iterator();		
		
		@Override
		public boolean hasNext() {
			return iter.hasNext();
		}

		@Override
		public K next() {
			return iter.next().getKey();
		}

		@Override
		public Iterator<K> iterator() {
			return new KeySet();
		}

		@Override
		public int size() {
			return HashMap.this.size();
		}
		
	}
	
	private class ValueCollection extends AbstractCollection<V> implements Iterator<V> {

		private Iterator<Map.Entry<K, V>> iter = new EntrySet().iterator();		
		
		@Override
		public boolean hasNext() {
			return iter.hasNext();
		}

		@Override
		public V next() {
			return iter.next().getValue();
		}

		@Override
		public Iterator<V> iterator() {
			return new ValueCollection();
		}

		@Override
		public int size() {
			return HashMap.this.size();
		}		
	}
}
