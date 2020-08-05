package il.co.ilrd.gateway_server;

import java.net.Socket;

public class TcpPeer implements Peer {

	Socket socket;
	
	public TcpPeer(Socket socket) {
		this.socket = socket;
	}


	@Override
	public void send(Response res) {
		// TODO Auto-generated method stub
		
	}

}
