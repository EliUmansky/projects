package il.co.ilrd.pair;

import java.util.Comparator;
import java.util.Map;

public class Pair<K, V> implements Map.Entry<K, V> {

	private K key;
	private V value;
	
	private Pair(K key, V value) {
		this.key = key;
		this.value = value;
	}
	public static<K, V> Pair<K, V> of(K key, V value) {
		return new Pair<K, V>(key, value);
	}

	public static<K, V> Pair<V, K> swap(Pair<K, V> pair) {
		return of(pair.getValue(), pair.getKey());		
	}

	public K setKey(K key) {
		K oldKey = this.getKey();
		this.key = key;
		
		return oldKey;
	}

	@Override
	public K getKey() {
		return key;
	}
	@Override
	public V getValue() {
		return value;
	}
	@Override
	public V setValue(V value) {
		V oldValue = this.getValue();
		this.value = value;
		
		return oldValue;
	}

	@Override
	public String toString() {
		return "Class Pair, Key=" + key + ", Value=" + value;
	}

	@Override
	public boolean equals(Object obj) {
        if (obj == this) { return true; }
        if (obj == null || obj.getClass() != this.getClass()) { return false; }

        @SuppressWarnings("unchecked")
		Pair<K, V> other = (Pair<K, V>) obj;
      
        return (this.getKey() == other.getKey() 
                     || (this.getKey() != null && this.getKey().equals(other.getKey())))
                && (this.getValue() == other.getValue() 
                     || (this.getValue() != null && this.getValue().equals(other.getValue())));
    }
	
	@Override
	public int hashCode() {
		int result = 17;
		result = 31 * result + ((key == null) ? 0 : key.hashCode());
		result = 31 * result + ((value == null) ? 0 : value.hashCode());
		
		return result;
	}

	public static<T extends Comparable<T>> Pair<T, T> minMax(T[] arr) {
		if (null == arr || 0 == arr.length) { return of(null, null); }
		if (1 == arr.length) { return of(arr[0], arr[0]); }
		
		T min = null;
		T max = null;
		int i;
		
		if (arr[0].compareTo(arr[1]) > 0) {
			max = arr[0];
			min = arr[1];
		} else {
			max = arr[1];
			min = arr[0];
		}
		
		for (i = 2; i < arr.length - 1; i += 2) {
			if (arr[i].compareTo(arr[i + 1]) > 0) {
				if (arr[i].compareTo(max) > 0) {
					max = arr[i];
				}
				if (arr[i + 1].compareTo(min) < 0) {
					min = arr[i + 1];
				}
			} else {
				if (arr[i + 1].compareTo(max) > 0) {
					max = arr[i + 1];
				}
				if (arr[i].compareTo(min) < 0) {
					min = arr[i];
				}
			}
		}
		
		if (i < arr.length) {
			if (arr[arr.length - 1].compareTo(max) > 0) {
				max = arr[arr.length - 1];
			}
			if (arr[arr.length - 1].compareTo(min) < 0) {
				min = arr[arr.length - 1];
			}
		}
		
		return of(min, max);
	}
	
	public static <T> Pair<T, T> minMax(T[] arr, Comparator<T> comp) {
		if (null == arr || 0 == arr.length) { return of(null, null); }
		if (1 == arr.length) { return of(arr[0], arr[0]); }
		
		T min = null;
		T max = null;
		int i;
		
		if (comp.compare(arr[0], arr[1]) > 0) {
			max = arr[0];
			min = arr[1];
		} else {
			max = arr[1];
			min = arr[0];
		}
		
		for (i = 2; i < arr.length - 1; i += 2) {
			if (comp.compare(arr[i], arr[i + 1]) > 0) {
				if (comp.compare(arr[i], max) > 0) {
					max = arr[i];
				}
				if (comp.compare(arr[i + 1], min) < 0) {
					min = arr[i + 1];
				}
			} else {
				if (comp.compare(arr[i + 1], max) > 0) {
					max = arr[i + 1];
				}
				if (comp.compare(arr[i], min) < 0) {
					min = arr[i];
				}
			}
		}
		
		if (i < arr.length) {
			if (comp.compare(arr[arr.length - 1], max) > 0) {
				max = arr[arr.length - 1];
			}
			if (comp.compare(arr[arr.length - 1], min) < 0) {
				min = arr[arr.length - 1];
			}
		}
		
		return of(min, max);
	}	
}
