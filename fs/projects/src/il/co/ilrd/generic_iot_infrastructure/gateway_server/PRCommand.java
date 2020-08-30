package il.co.ilrd.gateway_server;

import java.util.function.Function;

import org.bson.Document;

import com.google.gson.JsonObject;
import com.mongodb.MongoClient;
import com.mongodb.client.MongoCollection;
import com.mongodb.client.MongoDatabase;

public class PRCommand implements Command {
	MongoClient mongoClient;
	JsonObject data;
	
	public PRCommand(JsonObject data) {
		this.data = data;
	}
	@Override
	public Response execute() {
		try {
			mongoClient = new MongoClient("localhost", 27017);
			String databaseName = data.get("companyName").getAsString();
			MongoDatabase database = mongoClient.getDatabase(databaseName);
			MongoCollection<Document> productsCollection = database.getCollection("products"); 
			Document document = new Document("productName", data.get("productName").getAsString());
			productsCollection.insertOne(document);
		} catch (Exception e) {
			return new Response(500, "DB Error");
		}
		
		return new Response(200, "Collection created");
	}

	public static String getKey() {
		return "PR";
	}
	
	public static Function<JsonObject, Command> getCtorReference() {
		return PRCommand::new;
	}
}
