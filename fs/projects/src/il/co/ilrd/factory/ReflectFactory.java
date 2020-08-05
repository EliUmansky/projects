package il.co.ilrd.factory;

import java.util.HashMap;
import java.util.Map;
import java.lang.reflect.*;

public class ReflectFactory<K, T, D> {
	
	Map<K, Class<? extends T>> map_types = new HashMap<K, Class<? extends T>>();
	
	public void Add(K key, Class<? extends T> type) {
		map_types.put(key, type);
	}
	
	public T Create(K key) throws InstantiationException, IllegalAccessException, IllegalArgumentException, InvocationTargetException, NoSuchMethodException, SecurityException {
		return map_types.get(key).getConstructor().newInstance();
	}
	public T CreateWithArgs(K key, D data) throws InstantiationException, IllegalAccessException, IllegalArgumentException, InvocationTargetException, NoSuchMethodException, SecurityException {
		return map_types.get(key).getConstructor().newInstance(data);
	}
}
