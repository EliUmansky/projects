package il.co.ilrd.my_server;

import java.io.ByteArrayInputStream;
import java.io.ByteArrayOutputStream;
import java.io.DataOutputStream;
import java.io.IOException;
import java.io.ObjectInputStream;
import java.io.ObjectOutputStream;
import java.net.InetSocketAddress;
import java.net.ServerSocket;
import java.net.SocketException;
import java.nio.ByteBuffer;
import java.nio.channels.*;
import java.util.Iterator;
import java.util.Set;

public class TCPCommunication implements Communication {
	private volatile boolean flag = true;
	private ChatServer server;
	private Selector selector;
	private ServerSocketChannel serverChannel;
	private ServerSocket serverSocket;
	private SocketChannel channel;
	{
		try {
			selector = Selector.open();
			serverChannel = ServerSocketChannel.open();
			serverSocket =  serverChannel.socket();
			serverSocket.bind(new InetSocketAddress("localhost", 6699));
			serverChannel.configureBlocking(false);
			serverChannel.register(selector, SelectionKey.OP_ACCEPT);
		} catch (IOException e) {
			System.err.println(e.getMessage());
			e.printStackTrace();
		}
	}
	
	public TCPCommunication(ChatServer server) {
		this.server = server;
		new Thread(()->Init()).start();
	}
	
	interface ServerMethod {
        void invokeMethod(Request request);
    }

    private ServerMethod[] serverMethods = new ServerMethod[] {
        new ServerMethod() { public void invokeMethod(Request request) { 
        	RequestLogin req = (RequestLogin)request;
        	server.logIn(req.getMsgID(), req.getEmail(), req.getUserName(), new SocketPeer(channel));
        	}
        },
        new ServerMethod() { public void invokeMethod(Request request) { 
        	RequestCreateGroup req = (RequestCreateGroup)request;
        	server.createNewGroup(req.getMsgID(), req.getUserId(), req.getGroupName());
        	}
        },
        new ServerMethod() { public void invokeMethod(Request request) {
        	RequestJoinGroup req = (RequestJoinGroup)request;
        	server.joinGroup(req.getMsgID(), req.getUserId(), req.getGroupName());
        	} 
        },
        new ServerMethod() { public void invokeMethod(Request request) {
        	RequestLeaveGroup req = (RequestLeaveGroup)request;
        	server.leaveGroup(req.getMsgID(), req.getUserId() , req.getGroupName());
        	} 
        },
        new ServerMethod() { public void invokeMethod(Request request) { 
        	RequestSend req = (RequestSend)request;
        	server.sendMsg(req.getMsgID(), req.getUserId(), req.getGroupName() , req.getMsg());
        	} 
        },
        new ServerMethod() { public void invokeMethod(Request request) { 
        	System.out.println("server coomun logout");
        	RequestLogout req = (RequestLogout)request;
        	server.logOut(req.getMsgID(), req.getUserId());
        	} 
        }
    };

	@Override
	public void Init() {
		while (flag) {
			try {
				selector.select();
			} catch (IOException e) {
				//this is the blocking part
				e.printStackTrace();
			}
			Iterator<SelectionKey> keys = selector.selectedKeys().iterator();
			while(keys.hasNext()) {
				SelectionKey key = keys.next();
				if(key.isAcceptable()) {
					ServerSocketChannel server = (ServerSocketChannel) key.channel();
					try {
						SocketChannel channel = server.accept();
						channel.configureBlocking(false);
						channel.register(selector, SelectionKey.OP_READ);
					} catch (IOException e) {
						// TODO Auto-generated catch block
						e.printStackTrace();
					}
				}else if(key.isReadable()) {
					channel = (SocketChannel) key.channel();
					ByteBuffer buffer = ByteBuffer.allocate(1024);
					Request request = null;
					try {
						channel.read(buffer);
					} catch (IOException e1) {
						channel = (SocketChannel) key.channel();
						try {
							System.out.println("client fallen");
							channel.close();
						} catch (IOException e) {
							System.out.println("error");
							e.printStackTrace();
						}
						 key.cancel();
					     keys.remove();
					     continue;
					}
					try {
						ByteArrayInputStream bais = new ByteArrayInputStream(buffer.array());
						ObjectInputStream ois = new ObjectInputStream(bais);
						request = (Request) ois.readObject();
						buffer.clear();
				    } catch (IOException | ClassNotFoundException e) {
				        e.printStackTrace();
				    }
					serverMethods[request.getOpId().ordinal()].invokeMethod(request);
				}else {
					System.out.println("this is a bug!!!!");
				}
				keys.remove();
			}
		}
	}
	
	private class SocketPeer implements Peer {
		private SocketChannel clientSocket = null;

		public SocketPeer(SocketChannel clientSocket) {
			this.clientSocket = clientSocket;
		}
		
		@Override
		public void responseLogin(int msgID, int userID, Set<String> groupNames, Status status) {
			ResponseLogin res = new ResponseLogin(msgID, status, userID, groupNames);
			sendResponse(res);
		}
		
		@Override
		public void responseMessage(int msgID, int userID, String userName, String groupName, UsrProperties prop, String message, Status status) {	
			ResponseSend res = new ResponseSend(msgID, status, userID, groupName, message, userName, prop);
			sendResponse(res);
		}
		
		@Override
		public void responseCreateGroup(int msgID, String groupName, Status status) {
			ResponseCreateGroup res = new ResponseCreateGroup(msgID, status, groupName);
			sendResponse(res);
		}

		@Override
		public void responseJoinGroup(int msgID, int userID, String userName, String groupName, Status status) {
			ResponseJoinGroup res = new ResponseJoinGroup(msgID, status, userID, groupName, userName);
			sendResponse(res);
		}

		@Override
		public void responseLeaveGroup(int msgID, int userID, String userName, String groupName, Status status) {
			ResponseLeaveGroup res = new ResponseLeaveGroup(msgID, status, userID, groupName, userName);
			sendResponse(res);
		}
		@Override
		public void responseLogout(int msgID, Status status) {
			System.out.println("peer logout");
			ResponseLogout res = new ResponseLogout(msgID, status);
			sendResponse(res);
		}
		public void sendResponse(Response response) {
			try{
				ByteArrayOutputStream bos = new ByteArrayOutputStream();
				ObjectOutputStream oos = new ObjectOutputStream(bos);
				try {
					DataOutputStream outToServer = new DataOutputStream(clientSocket.socket().getOutputStream());
				}catch(SocketException e) {
					System.out.println("need to think about how to deal with the socket");
					return;
				}
				oos.writeObject(response);
				oos.flush();
				ByteBuffer bytebuffer = ByteBuffer.wrap(bos.toByteArray());
				
				clientSocket.write(bytebuffer);
			} catch (IOException e) {
				e.printStackTrace();
			}
		}

	}
	
	public static void main(String [] args) {
		ChatServer server = new ChatServerHub();
		new TCPCommunication(server);
	}
}