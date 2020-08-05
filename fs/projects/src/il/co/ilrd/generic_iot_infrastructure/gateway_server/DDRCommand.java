package il.co.ilrd.gateway_server;

import java.util.function.Function;

import com.google.gson.JsonObject;

public class DDRCommand implements Command {
	
	JsonObject data;
	
	public DDRCommand(JsonObject data) {
		this.data = data;
	}

	@Override
	public Response execute() {
		return new Response(200, "perfect");
	}

	public static String getKey() {
		return "DDR";
	}
	
	public static Function<JsonObject, Command> getCtorReference() {
		return DDRCommand::new;
	}
}