package il.co.ilrd.gateway_server;

import java.util.function.Function;

import org.bson.Document;

import com.google.gson.JsonObject;
import com.mongodb.MongoClient;
import com.mongodb.client.MongoCollection;
import com.mongodb.client.MongoDatabase;

public class IOTCommand implements Command {
	MongoClient mongoClient;
	JsonObject data;
	
	public IOTCommand(JsonObject data) {
		this.data = data;
	}
	@Override
	public Response execute() {
		try {
			String companyName = data.get("companyName").getAsString();
			String productName = data.get("productName").getAsString();
			long serialNumber = data.get("serialNumber").getAsLong();
			mongoClient = new MongoClient("localhost", 27017);
			MongoDatabase database = mongoClient.getDatabase(companyName);			
			MongoCollection<Document> collection = database.getCollection(productName + serialNumber);
			String iotData = data.get("data").toString();
			Document doc = new Document("DateTime", System.currentTimeMillis())
	                .append("data", Document.parse(iotData));
			collection.insertOne(doc);
		} catch (Exception e) {
			e.printStackTrace();
			return new Response(500, "DB Error");
		}
		return new Response(200, "The DB was updated");
	}

	public static String getKey() {
		return "IOT";
	}
	
	public static Function<JsonObject, Command> getCtorReference() {
		return IOTCommand::new;
	}
}
