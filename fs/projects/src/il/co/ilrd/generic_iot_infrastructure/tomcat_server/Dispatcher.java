package il.co.ilrd.tomcat_server;

import java.util.LinkedList;
import java.util.List;
import java.util.function.Consumer;

public class Dispatcher<T> {
	
	private List<Callback<T>> list = new LinkedList<>();
	
	public void register(Callback<T> callback) {
		callback.dispatcher = this;
		list.add(callback);
	}
	
	public void stopAll() {
		for(Callback<T> cb: list) {
			cb.notifyDeath();
		}
		
		list.clear();
	}
	
	public void updateAll(T msg) {
		for(Callback<T> cb : list) {
			cb.update(msg);
		}
	}
	
	public static class Callback <T>{
		private Consumer<T> consumer;
		private Runnable runnable;
		private Dispatcher<T> dispatcher;
		
		public Callback(Consumer<T> consumer, Runnable runnable) {
			this.consumer = consumer;
			this.runnable = runnable;
		}
		
		private void update(T msg) { 
			consumer.accept(msg);
		}	
		
		private void notifyDeath() {  			
			runnable.run();
			dispatcher = null;
		}
		
		public void stopService() {
			try {
				dispatcher.list.remove(this);					
			} catch (NullPointerException e) {
				System.out.println("You're not signed to the service");
			}
			
			dispatcher = null;
		}
	}
}
