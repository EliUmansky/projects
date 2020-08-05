package il.co.ilrd.tomcat_server;

import java.sql.ResultSet;
import java.sql.SQLException;

import com.google.gson.JsonArray;
import com.google.gson.JsonObject;

public class ResultSetToJsonConverter {
	
	public static JsonObject parseCompanies(ResultSet rs) throws SQLException  {
		rs.next();
		JsonObject json = new JsonObject();		
		
		json.addProperty("companyId", rs.getInt("company_id"));
		json.addProperty("companyName", rs.getString("company_name"));
		json.addProperty("email", rs.getString("email"));
		json.addProperty("password", rs.getString("password"));
		json.addProperty("address", rs.getString("address"));

		return json;
	}
	
	public static JsonObject parseProducts(ResultSet rs) throws SQLException  {
		JsonArray jsonArray = new JsonArray();
		JsonObject json = new JsonObject();
		
		while (rs.next()) {
			JsonObject jsonObj = new JsonObject();
			jsonObj.addProperty("productId", rs.getInt("product_id"));
			jsonObj.addProperty("productName", rs.getString("product_name"));			
			jsonArray.add(jsonObj);		
		}

		json.add("products", jsonArray);
		
		return json;
	}
}
