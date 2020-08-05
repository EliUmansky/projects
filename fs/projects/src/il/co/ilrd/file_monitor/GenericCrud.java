package il.co.ilrd.crud;

import java.io.Serializable;

public interface GenericCrud<K extends Serializable, T extends Serializable> extends AutoCloseable {
	K create(T obj);
	T read(K key);
	void update(K key, T obj);
	T delete(K key);
}
