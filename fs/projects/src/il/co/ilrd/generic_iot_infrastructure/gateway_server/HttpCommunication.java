package il.co.ilrd.gateway_server;

import java.io.BufferedReader;
import java.io.IOException;
import java.io.InputStreamReader;
import java.net.InetSocketAddress;

import com.google.gson.JsonObject;
import com.google.gson.JsonParser;
import com.sun.net.httpserver.HttpContext;
import com.sun.net.httpserver.HttpExchange;
import com.sun.net.httpserver.HttpServer;

public class HttpCommunication implements Communication {
	
	private HttpServer server;
	private TaskManagement taskManagement;
	
	public HttpCommunication(TaskManagement taskManagement) {
		this.taskManagement = taskManagement;
	}

	@Override
	public void listen(String ip, int port) {
		try {
			server = HttpServer.create(new InetSocketAddress(ip, port), 0);
		} catch (IOException e) {
			e.printStackTrace();
		}
	    HttpContext context = server.createContext("/");
	    context.setHandler(this::handleRequest);
	    server.start();		
	}

	private void handleRequest(HttpExchange exchange) throws IOException {
		Peer peer = new HttpPeer(exchange);
		if (!exchange.getRequestMethod().equals("POST")) {
			peer.send(new Response(400, "Bad Request"));
			return;
		}
		
		JsonObject data = getbody(exchange);
		taskManagement.createAndSubmitTask(peer, data);
	}

	private JsonObject getbody(HttpExchange exchange) {
		StringBuilder str = null;

// FIXME: address not json body (exception)
		try (InputStreamReader isr = new InputStreamReader(exchange.getRequestBody(),"utf-8");
			     BufferedReader br = new BufferedReader(isr);) {
			String line;
			str = new StringBuilder(512);
			while ((line = br.readLine()) != null) {
				str.append(line);
			}			
		} catch (IOException e) {
			e.printStackTrace();			
		} 

		return JsonParser.parseString(str.toString()).getAsJsonObject();
	}
	
}
