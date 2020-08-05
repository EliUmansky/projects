package il.co.ilrd.gateway_server;

import java.util.function.Function;

import com.google.gson.JsonObject;

public class IOTCommand implements Command {
	
	JsonObject data;
	
	public IOTCommand(JsonObject data) {
		this.data = data;
	}
	@Override
	public Response execute() {
		return new Response(404, "Not Found");
	}

	public static String getKey() {
		return "IOT";
	}
	
	public static Function<JsonObject, Command> getCtorReference() {
		return IOTCommand::new;
	}
}
