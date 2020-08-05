package il.co.ilrd.factory;

import java.util.HashMap;
import java.util.Map;
import java.util.function.Function;

public class Factory<K, T, D> {
	
	Map<K, Function<D, ? extends T>> mapTypes = new HashMap<K, Function<D, ? extends T>>();
	
	public void add(K key, Function<D, ? extends T> ctorFunc) {
		mapTypes.put(key, ctorFunc);
	}
	
	public T create(K key, D data) {
		try {
			return mapTypes.get(key).apply(data);			
		} catch (NullPointerException e) {
			return null;
		}
	}

}
