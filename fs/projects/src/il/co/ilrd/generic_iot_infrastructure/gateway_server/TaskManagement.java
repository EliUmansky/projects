package il.co.ilrd.gateway_server;

import java.util.concurrent.ExecutorService;

import com.google.gson.JsonObject;

public class TaskManagement {
	private ExecutorService threadPool;
	
	public TaskManagement(ExecutorService threadPool) {
		this.threadPool = threadPool;
	}

	public void createAndSubmitTask(Peer peer, JsonObject data) {
		Task task = createTask(peer, data);
		submitTask(task);
	}
	
	private Task createTask(Peer peer, JsonObject data) {

		String key = data.get("key").getAsString();
		JsonObject internalData = data.get("data").getAsJsonObject();
		
		return new Task(key, internalData, peer);
	}
	
	private void submitTask(Task task) {
		threadPool.submit(task);
	}
}
