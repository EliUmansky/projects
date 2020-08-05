package il.co.ilrd.chat_server_project;

import java.io.ByteArrayInputStream;
import java.io.ByteArrayOutputStream;
import java.io.IOException;
import java.io.ObjectInputStream;
import java.io.ObjectOutputStream;
import java.net.InetSocketAddress;
import java.net.ServerSocket;
import java.nio.ByteBuffer;
import java.nio.channels.SelectionKey;
import java.nio.channels.Selector;
import java.nio.channels.ServerSocketChannel;
import java.nio.channels.SocketChannel;
import java.util.Iterator;
import java.util.Set;

public class TCPCommunication implements Communication {
	
	private ChatServer server;
	private ServerSocketChannel serverChannel;
	private ServerSocket serverSocket;
	private Selector selector = Selector.open();
	private SocketChannel channel = null;
	
	public TCPCommunication(ChatServer server, int portNumber) throws IOException {
		this.server = server;
		serverChannel = ServerSocketChannel.open();
		serverSocket = serverChannel.socket();
		serverSocket.bind(new InetSocketAddress("0.0.0.0", 6668));
		serverChannel.configureBlocking(false);
		serverChannel.register(selector, SelectionKey.OP_ACCEPT);
		new Thread(()->Init()).start();
	}
	public static void main(String[] args) throws IOException {
		ChatServer server = new ChatServerHub();
		new TCPCommunication(server, 6668);
		
	}

	@Override
	public void Init() {
		Iterator<SelectionKey> keys = null;
		SelectionKey key = null;
		
		while (true) {
			try {
				selector.select();
			} catch (IOException e) {
				e.printStackTrace();
			}
			
			keys = selector.selectedKeys().iterator();
			while(keys.hasNext()) {
				key = keys.next();
				keys.remove();
				if(key.isAcceptable()) {
					serverChannel = (ServerSocketChannel) key.channel();
					try {
						channel = serverChannel.accept(); 
						channel.configureBlocking(false);
						channel.register(selector, SelectionKey.OP_READ, ByteBuffer.allocate(1024));
					} catch (IOException e) {
						e.printStackTrace();
					}
					
				}else if(key.isReadable()) {
					channel = (SocketChannel)key.channel();
					ByteBuffer buffer = (ByteBuffer)key.attachment();
					try {
						channel.read(buffer);	

			            ObjectInputStream ois = new ObjectInputStream(new ByteArrayInputStream(buffer.array()));
			            Request req = null;
						try {
							req = (Request)ois.readObject();
						} catch (ClassNotFoundException | ClassCastException e) {
							continue;
						}
			            buffer.clear();				
			            serverMethods[req.getOpId().ordinal()].invokeMethod(req);
				         
					} catch (IOException e) {
						key.cancel();
						continue;
					}
				}
			}
		}
	}
	
	interface ServerMethod { void invokeMethod(Request req); }
	
	private ServerMethod[] serverMethods = new ServerMethod[] {
			new ServerMethod() { public void invokeMethod(Request req) { 
				server.logIn(req.getMsgID(), ((RequestLogin)req).getEmail(), ((RequestLogin)req).getUserName(), new SocketPeer(channel));
			} },
			new ServerMethod() { public void invokeMethod(Request req) { 
				server.createNewGroup(req.getMsgID(), ((RequestCreateGroup)req).getUserId(), ((RequestCreateGroup)req).getGroupName());
			} },
			new ServerMethod() { public void invokeMethod(Request req) { 
				server.joinGroup(req.getMsgID(), ((RequestJoinGroup)req).getUserId(), ((RequestJoinGroup)req).getGroupName());
			} },
			new ServerMethod() { public void invokeMethod(Request req) { 
				server.leaveGroup(req.getMsgID(), ((RequestLeaveGroup)req).getUserId(), ((RequestLeaveGroup)req).getGroupName());
			} },
			new ServerMethod() { public void invokeMethod(Request req) { 
				server.sendMsg(req.getMsgID(), ((RequestSend)req).getUserId(), ((RequestSend)req).getGroupName(), ((RequestSend)req).getMsg());
			} },
			new ServerMethod() { public void invokeMethod(Request req) { 
				server.logOut(req.getMsgID(), ((RequestLogout)req).getUserId());
			} }
	};
	
  
	class SocketPeer implements PeerResponse {
		private SocketChannel clientSocket = null;

		public SocketPeer(SocketChannel clientSocket) {
			this.clientSocket = clientSocket;
		}

		@Override
		public void responseMessage(int msgID, int userID, String userName, String groupName, UsrProperties prop,
				String message, Status status) {
			if (clientSocket.isConnected()) {
				ResponseSend response = new ResponseSend(chatOps.SEND_MSG, msgID, status, userID, groupName, message, userName, prop);
				SendResponse(response);
			}
		}

		@Override
		public void responseJoinGroup(int msgID, int userID, String userName, String groupName, Status status) {
			if (clientSocket.isConnected()) {
				ResponseJoinGroup response = new ResponseJoinGroup(chatOps.JOIN_GROUP, msgID, status, userID, groupName, userName);
				SendResponse(response);				
			}
		}

		@Override
		public void responseLogin(int msgID, int userID, Set<String> groupNames, Status status) {
			if (clientSocket.isConnected()) {
				ResponseLogin response = new ResponseLogin(chatOps.LOGIN, msgID, status, userID, groupNames);
				SendResponse(response);				
			}
		}

		@Override
		public void responseCreateGroup(int msgID, String groupName, Status status) {
			if (clientSocket.isConnected()) {
				ResponseCreateGroup response = new ResponseCreateGroup(chatOps.CREATE_GROUP, msgID, status, groupName);
				SendResponse(response);				
			}
		}

		@Override
		public void responseLeaveGroup(int msgID, int userID, String userName, String groupName, Status status) {
			if (clientSocket.isConnected()) {
				ResponseLeaveGroup response = new ResponseLeaveGroup(chatOps.LEAVE_GROUP, msgID, status, userID, groupName, userName);
				SendResponse(response);				
			}
		}
		
		@Override
		public void responseLogout(int msgID, int userID, Status status) {
			if (clientSocket.isConnected()) {
				ResponseLogout response = new ResponseLogout(chatOps.LOGOUT, msgID, status, userID);
				SendResponse(response);					
			}
		}
		
		public void SendResponse(Response response) {
			ByteArrayOutputStream bos = null;
			ObjectOutputStream out = null;
			try {
				bos = new ByteArrayOutputStream();
				out = new ObjectOutputStream(bos);
				out.writeObject(response);
				out.flush();
				ByteBuffer bb = ByteBuffer.wrap(bos.toByteArray()); 
				clientSocket.write(bb);
			} catch (IOException e) {
				try {
					clientSocket.close();
					return;
				} catch (IOException e1) {
					e1.printStackTrace();
				}
			}
		}
	}
}
