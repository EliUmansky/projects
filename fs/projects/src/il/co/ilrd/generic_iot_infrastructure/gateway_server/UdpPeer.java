package il.co.ilrd.gateway_server;

import java.net.DatagramSocket;

public class UdpPeer implements Peer {

	DatagramSocket socket;
	
	public UdpPeer(DatagramSocket socket) {
		this.socket = socket;
	}

	@Override
	public void send(Response res) {
		// TODO Auto-generated method stub
		
	}

}
