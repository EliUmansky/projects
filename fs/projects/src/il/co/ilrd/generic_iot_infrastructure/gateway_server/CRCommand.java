package il.co.ilrd.gateway_server;

import java.util.function.Function;

import com.google.gson.JsonObject;
import com.mongodb.MongoClient;
import com.mongodb.MongoNamespace;
import com.mongodb.client.MongoDatabase;

public class CRCommand implements Command {
	MongoClient mongoClient;
	JsonObject data;
	
	public CRCommand(JsonObject data) {
		this.data = data;
	}

	@Override
	public Response execute() {
		try {
			mongoClient = new MongoClient("localhost", 27017);
			String newDBName = data.get("companyName").getAsString();
			MongoNamespace.checkDatabaseNameValidity(newDBName);
			MongoDatabase database = mongoClient.getDatabase(newDBName);
			database.createCollection("users");
			database.createCollection("products");
		} catch (Exception e) {
			return new Response(500, "DB Error");
		}
		
		return new Response(200, "DB Created");
	}

	public static String getKey() {
		return "CR";
	}
	
	public static Function<JsonObject, Command> getCtorReference() {
		return CRCommand::new;
	}
}
