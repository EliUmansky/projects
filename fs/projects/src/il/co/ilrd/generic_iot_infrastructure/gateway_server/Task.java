package il.co.ilrd.gateway_server;

import com.google.gson.JsonObject;

public class Task implements Runnable {
	
	private Peer peer;
	private String key;
	private JsonObject data;
	
	public Peer getPeer() {
		return peer;
	}

	public String getKey() {
		return key;
	}

	public JsonObject getData() {
		return data;
	}

	public Task(String key, JsonObject data, Peer peer) {
		this.key = key;
		this.data = data;
		this.peer = peer;
	}

	@Override
	public void run() {
		Command command = CommandFactory.getInstance().create(key, data);
		peer.send(command.execute());
	}
}
