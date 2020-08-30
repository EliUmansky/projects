package il.co.ilrd.gateway_server;

import java.util.function.Function;

import org.bson.Document;

import com.google.gson.JsonObject;
import com.mongodb.MongoClient;
import com.mongodb.MongoNamespace;
import com.mongodb.client.MongoDatabase;
import static com.mongodb.client.model.Filters.*;

public class EURCommand implements Command {
	MongoClient mongoClient;
	JsonObject data;
	
	public EURCommand(JsonObject data) {
		this.data = data;
	}
	@Override
	public Response execute() {
		try {
			mongoClient = new MongoClient("localhost", 27017);
			String databaseName = data.get("companyName").getAsString();
			MongoNamespace.checkDatabaseNameValidity(databaseName);
			MongoDatabase database = mongoClient.getDatabase(databaseName);			
			String productName = data.get("productName").getAsString();
			Document productInDB = database.getCollection("products").find(eq("productName", productName)).first();
			if (null == productInDB) {
				return new Response(500, "Product is not registered");
			}
			
			String serialNumber = data.get("serialNumber").getAsString();			
			String collectionName = productName + serialNumber;
			MongoNamespace.checkCollectionNameValidity(collectionName);
			database.getCollection(collectionName);
			String userData = data.get("userData").toString();
			Document newUser = Document.parse(userData);
			database.getCollection("users").insertOne(newUser);	
		} catch (Exception e) {
			e.printStackTrace();
			return new Response(500, "DB Error");
		}
		return new Response(200, "The device was successfully registered");
	}

	public static String getKey() {
		return "EUR";
	}
	
	public static Function<JsonObject, Command> getCtorReference() {
		return EURCommand::new;
	}
}
