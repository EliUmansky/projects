package il.co.ilrd.tomcat_server;

import java.io.BufferedReader;

import javax.servlet.http.HttpServletRequest;

import com.google.gson.JsonObject;
import com.google.gson.JsonParser;

public class HttpRequestToJson {

	public static JsonObject parse(HttpServletRequest request) {
		StringBuffer str = new StringBuffer();
		String line = null;
		
		try {
			BufferedReader reader = request.getReader();
			while ((line = reader.readLine()) != null) {
				str.append(line);					
			}
		} catch (Exception e) { return null; }
		
		return JsonParser.parseString(str.toString()).getAsJsonObject();
	}
}
