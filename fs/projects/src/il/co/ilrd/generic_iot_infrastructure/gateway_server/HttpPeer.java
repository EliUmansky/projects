package il.co.ilrd.gateway_server;

import java.io.IOException;
import java.io.OutputStream;

import com.sun.net.httpserver.HttpExchange;

public class HttpPeer implements Peer {

	private HttpExchange exchange;
		
	public HttpPeer(HttpExchange exchange) {
		this.exchange = exchange;
	}

	public HttpExchange getExchange() {
		return exchange;
	}

	@Override
	public void send(Response res) {	
		OutputStream os = null;
	    try {
			exchange.sendResponseHeaders(res.getStatusCode(), res.getMsg().getBytes().length);
			os = exchange.getResponseBody();
			os.write(res.getMsg().getBytes());
			os.close();
		} catch (IOException e) {
			e.printStackTrace();
		}
	}
}
