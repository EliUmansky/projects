package il.co.ilrd.tomcat_server;

import com.google.gson.JsonElement;
import com.google.gson.JsonObject;

public class ProductDetails {
	private Integer productId;
	private String productName;
	
	private ProductDetails(JsonObject json) throws NullPointerException {
		JsonElement productIdElement = json.get("productId");
		JsonElement productNameElement = json.get("productName");
	
		if (null != productIdElement) {
			productId = Integer.parseInt(productIdElement.getAsString());	
		}
		if (null != productNameElement) {
			productName = json.get("productName").getAsString();			
		}
	}

	public static ProductDetails getProductDetails(JsonObject json) {
		try {
			return new ProductDetails(json);			
		} catch (NullPointerException e) {
			return null;
		}
	}

	public Integer getProductId() {
		return productId;
	}

	public String getProductName() {
		return productName;
	}

}
