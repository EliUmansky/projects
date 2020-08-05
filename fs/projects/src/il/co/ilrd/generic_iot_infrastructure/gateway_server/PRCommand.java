package il.co.ilrd.gateway_server;

import java.util.function.Function;

import com.google.gson.JsonObject;

public class PRCommand implements Command {
	
	JsonObject data;
	
	public PRCommand(JsonObject data) {
		this.data = data;
	}
	@Override
	public Response execute() {
		return new Response(300, "could be better");
	}

	public static String getKey() {
		return "PR";
	}
	
	public static Function<JsonObject, Command> getCtorReference() {
		return PRCommand::new;
	}
}
