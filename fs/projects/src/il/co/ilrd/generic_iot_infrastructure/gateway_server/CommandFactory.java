package il.co.ilrd.gateway_server;

import com.google.gson.JsonObject;

public class CommandFactory extends Factory<String, Command, JsonObject> {
	
	private static final CommandFactory factory = new CommandFactory();		

	public static CommandFactory getInstance() {
		return factory;
	}
	
	public static int size() {
		return factory.mapTypes.size();
	}
}
