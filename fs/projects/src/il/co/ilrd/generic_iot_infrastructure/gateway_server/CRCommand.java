package il.co.ilrd.gateway_server;

import java.util.function.Function;

import com.google.gson.JsonObject;

public class CRCommand implements Command {
	
	JsonObject data;
	
	public CRCommand(JsonObject data) {
		this.data = data;
	}

	@Override
	public Response execute() {
		return new Response(200, "perfect");
	}

	public static String getKey() {
		return "CR";
	}
	
	public static Function<JsonObject, Command> getCtorReference() {
		return CRCommand::new;
	}
}
