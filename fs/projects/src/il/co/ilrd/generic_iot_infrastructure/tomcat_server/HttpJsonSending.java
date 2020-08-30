package il.co.ilrd.tomcat_server;

import java.io.IOException;
import java.io.OutputStream;
import java.net.HttpURLConnection;
import java.net.URL;

public class HttpJsonSending {
	public static void sendHTTPRequest(String urlAddress, String json) throws IOException {
		URL url = new URL(urlAddress);
		HttpURLConnection con = (HttpURLConnection) url.openConnection();
		con.setRequestMethod("POST");
		con.setDoOutput(true);
		con.setDoInput(true);
		con.setRequestProperty("Content-Type", "application/json; utf-8");
		con.setRequestProperty("Content-Length", Integer.toString(json.length()));
		con.setUseCaches(false);

		OutputStream os = con.getOutputStream();
		byte[] input = json.getBytes("utf-8");
		con.connect();
		os.write(input, 0, input.length);
		con.getInputStream();
		con.disconnect();		
	}
}
