package il.co.ilrd.tomcat_server;

import com.google.gson.JsonElement;
import com.google.gson.JsonObject;

public class CompanyDetails {
	private Integer companyId;
	private String email;
	private String encriptedPassword;
	private String companyName;
	private String address;
	
	private CompanyDetails(JsonObject json) throws NullPointerException {
		JsonElement companyIdElement = json.get("companyId");
		if (null != companyIdElement) {
			companyId = Integer.parseInt(companyIdElement.getAsString());			
		}
		email = json.get("email").getAsString();
		encriptedPassword = Encriptor.encript(json.get("password").getAsString());
		companyName = json.get("companyName").getAsString();
		JsonElement addressElement = json.get("address");
		if (null != addressElement) {
			address = addressElement.getAsString();
		}
	}
	
	public static CompanyDetails getCompanyDetails(JsonObject json) {
		try {
			return new CompanyDetails(json);			
		} catch (NullPointerException e) {
			return null;
		}
	}
	
	public static JsonObject getJson(CompanyDetails details) {
		JsonObject jsonObject = new JsonObject();		
		jsonObject.addProperty("companyId", details.getCompanyId());
		jsonObject.addProperty("companyName", details.getCompanyName());
		jsonObject.addProperty("email", details.getEmail());
		jsonObject.addProperty("password", details.getEncriptedPassword());
		jsonObject.addProperty("address", details.getAddress());

		return jsonObject;
	}
	
	public Integer getCompanyId() {
		return companyId;
	}
	
	public String getEmail() {
		return email;
	}

	public String getEncriptedPassword() {
		return encriptedPassword;
	}

	public String getCompanyName() {
		return companyName;
	}

	public String getAddress() {
		return address;
	}
}
