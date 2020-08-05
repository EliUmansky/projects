package il.co.ilrd.gateway_server;

import com.google.gson.JsonObject;

public interface Server {
	public Task createTask(Peer peer, JsonObject data);
}
