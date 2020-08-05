package il.co.ilrd.gateway_server;

import java.util.function.Function;

import com.google.gson.JsonObject;

public class EURCommand implements Command {
	
	JsonObject data;
	
	public EURCommand(JsonObject data) {
		this.data = data;
	}
	@Override
	public Response execute() {
		return new Response(501, "There were some problems with the server, sorry :/");
	}

	public static String getKey() {
		return "EUR";
	}
	
	public static Function<JsonObject, Command> getCtorReference() {
		return EURCommand::new;
	}
}
