package il.co.ilrd.tomcat_server;

public interface GenericCrud<K, T> extends AutoCloseable {
	K create(T obj);
	T read(K key);
	void update(K key, T obj);
	T delete(K key);
}
