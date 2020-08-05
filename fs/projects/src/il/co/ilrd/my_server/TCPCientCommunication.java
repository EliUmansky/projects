package il.co.ilrd.my_server;

import java.io.ByteArrayOutputStream;
import java.io.DataInputStream;
import java.io.DataOutputStream;
import java.io.IOException;
import java.io.ObjectInputStream;
import java.io.ObjectOutputStream;
import java.net.Socket;
import java.net.UnknownHostException;
import java.util.InputMismatchException;
import java.util.Scanner;

public class TCPCientCommunication {
	private Client client;
	private Socket clientSocket ;
	private volatile boolean flag = true;
	
	
	public TCPCientCommunication(String host , int port, Client client) throws UnknownHostException, IOException {
		clientSocket = new Socket(host, port);
		this.client = client;
		init();
	}
	
	interface ResponseMethod{
		void invoke(Response res);
	}
	
	ResponseMethod[] responseMethod = new ResponseMethod[] {
			new ResponseMethod() {public void invoke(Response res) {
				if(res.getStatus() == Status.SUCCESS) {
					ResponseLogin response = (ResponseLogin)res;
					client.loginResponse(response.getUserId(), response.getGroups());
				}
			}
			},
			new ResponseMethod() {public void invoke(Response res) {
				ResponseCreateGroup response = (ResponseCreateGroup)res;
				client.createGroupResponse(response.getGroupName(), res.getStatus());
			}
			},
			new ResponseMethod() {public void invoke(Response res) {
				ResponseJoinGroup response = (ResponseJoinGroup)res;
				client.joinGroupResponse(response.getUserId(), response.getSenderName(), response.getGroupName(), res.getStatus());
			}
			},
			new ResponseMethod() {public void invoke(Response res) {
				ResponseLeaveGroup response = (ResponseLeaveGroup)res;
				client.leaveGroupResponse(response.getUserId(), response.getSenderName(), response.getGroupName(), res.getStatus());
			}
			},
			new ResponseMethod() {public void invoke(Response res) {
				ResponseSend response = (ResponseSend)res;
				client.messageResponse(response.getUserId(), response.getSenderName(), response.getGroupName(), response.getMsg(), response.getProp(), res.getStatus());
			}
			},
			new ResponseMethod() {public void invoke(Response res) {
				System.out.println("ResponseLogout");
				ResponseLogout response = (ResponseLogout)res;
				flag = false;
				client.logOutResponse(response.getUserId());
			}
			}
		};
	
	interface ClientMethod {
        void invokeMethod();
    }
	private void sendRequest(Request rec) {
		ByteArrayOutputStream bos = new ByteArrayOutputStream();
		DataOutputStream outToServer = null;
		try {
			outToServer = new DataOutputStream(clientSocket.getOutputStream());
			ObjectOutputStream oos = new ObjectOutputStream(bos);
			oos.writeObject(rec);
			oos.flush();
			byte[] byteArr = bos.toByteArray();
			outToServer.write(byteArr);
		} catch (IOException e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
		}
		
	}
	private ClientMethod[] clientRequestMethods = new ClientMethod[] {
			new ClientMethod() { public void invokeMethod() { 
	        	RequestLogin rec = new RequestLogin(client.getEmail(), client.getName());
	        	sendRequest(rec);
	        	}
	        },
			new ClientMethod() { public void invokeMethod() { 
	        	Scanner scanner = new Scanner(System.in);
	        	System.out.println("enter the group name you want to create");
	        	String groupName = scanner.nextLine();
	        	RequestCreateGroup rec = new RequestCreateGroup(client.getUserID(), groupName);
	        	sendRequest(rec);
				}
	        },
			new ClientMethod() { public void invokeMethod() { 
	        	Scanner scanner = new Scanner(System.in);
	        	System.out.println("enter the group name you want to join");
	        	String groupName = scanner.nextLine();
	        	RequestJoinGroup rec = new RequestJoinGroup(client.getUserID(), groupName);
	        	sendRequest(rec);
				}
	        },
			new ClientMethod() { public void invokeMethod() { 
	        	Scanner scanner = new Scanner(System.in);
	        	System.out.println("enter the group name you want to leave");
	        	String groupName = scanner.nextLine();
	        	RequestLeaveGroup rec = new RequestLeaveGroup(client.getUserID(), groupName);
	        	sendRequest(rec);
				}
	        },
			new ClientMethod() { public void invokeMethod() { 
	        	Scanner scanner = new Scanner(System.in);
	        	System.out.println("enter the groupname");
	        	String groupName = scanner.nextLine();
	        	System.out.println("enter the message");
	        	String msg = scanner.nextLine();
	        	RequestSend rec = new RequestSend(client.getUserID(), groupName, msg);
	        	sendRequest(rec);
				}
	        },
			new ClientMethod() { public void invokeMethod() { 
				System.out.println("clientLogoutMethod");
	        	RequestLogout rec = new RequestLogout(client.getUserID());
	        	sendRequest(rec);
				}
	        }
			
	};
	//this is the thread of receive
	public void init() {
		new Thread(()->{
			while(flag) {
				Response response = null; 
				try {
					DataInputStream dis = new DataInputStream(clientSocket.getInputStream());
					ObjectInputStream ois = new ObjectInputStream(dis);
					response = (Response) ois.readObject();
				} catch (IOException | ClassNotFoundException e) {
					System.out.println(e.getMessage());
					e.printStackTrace();
				}
				responseMethod[response.getOpId().ordinal()].invoke(response);
			}
		}).start();
		
		while(flag) {
			System.out.println("enter the procedure");
			System.out.println("0 - login");
			System.out.println("1 - create group");
			System.out.println("2 - join group");
			System.out.println("3 - leave group");
			System.out.println("4 - send message");
			System.out.println("5 - logout");
			Scanner scanner = new Scanner(System.in);
			Integer input = null;
			try {
				input = scanner.nextInt();
				if(flag == false) {
					return;
				}
			}catch(InputMismatchException ex) {
				System.out.println("only digits please");
				continue;
			}
			if( input > 5 || input < 0){
				System.out.println("only 0 to 5 is accepted");
				continue;
			}else {
				clientRequestMethods[input].invokeMethod();
			}
		}
	}
	
	public static void main(String argv[]) throws Exception {	
		
		TCPCientCommunication client = new TCPCientCommunication("10.1.0.22", 6699, new Client());
	}
}
		 
					  
			  
			  
		
