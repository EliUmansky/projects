package il.co.ilrd.chat_server_project;

import java.io.ByteArrayOutputStream;
import java.io.DataInputStream;
import java.io.DataOutputStream;
import java.io.IOException;
import java.io.ObjectInputStream;
import java.io.ObjectOutputStream;
import java.net.Socket;
import java.net.UnknownHostException;
import java.util.ArrayList;
import java.util.List;
import java.util.Scanner;


public class MyClient2 {
	private Socket clientSocket;
	private volatile boolean loggedIn = true;
	private int userId;
	private List<String> userGroups;
	Thread responseThread = new Thread(()->getResponse());
	
	public MyClient2() {
		userGroups = new ArrayList<>();
		try {
			clientSocket = new Socket("localhost", 6668);
		} catch (IOException e) {
			e.printStackTrace();
		}
		responseThread.start();
		Login();
	}

	public void Start() {
		Scanner scanner = new Scanner(System.in);
		String numericRegex = "-?\\d+(\\.\\d+)?";
		String input;
		int requestIndex = 0;
		
		System.out.println("Please enter your choice:");
		System.out.println("1 - Create New Group\n2 - Join Group\n3 - Leave roup\n4 - Send Message\n5 - Log Out\n");
		while (loggedIn) {
			input = scanner.nextLine();			
			if (input.matches(numericRegex)) {
				requestIndex = Integer.parseInt(input);
				if (1 > requestIndex || 5 < requestIndex) {
					System.out.println("Wrong input, enter a valid one");
					continue;
				}				
			} else {
				System.out.println("Wrong input, enter a valid one");
				continue;
			}
			
			sendRequest[requestIndex].invokeMethod();
		}			
		scanner.close();
	}

	/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */
	/*          	R  E  Q  U  E  S  T      M  E  T  H  O  D  S                       */
	/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */

	interface RequestMethod { void invokeMethod(); }
	
	private RequestMethod[] sendRequest = new RequestMethod[] {
			new RequestMethod() { public void invokeMethod() { 
				Login();
			} },
			new RequestMethod() { public void invokeMethod() { 
				CreateNewGroup();
			} },
			new RequestMethod() { public void invokeMethod() { 
				JoinGroup();
			} },
			new RequestMethod() { public void invokeMethod() { 
				LeaveGroup();
			} },
			new RequestMethod() { public void invokeMethod() { 
				SendMsg();
			} },
			new RequestMethod() { public void invokeMethod() { 
				Logout();
			} },
	};
	
	public void Login() {
		Scanner scanner = new Scanner(System.in);
		
		String email;
		String userName;
		System.out.println("Please enter your email:");
		email = scanner.nextLine();
		System.out.println("Please enter your user name:");
		userName = scanner.nextLine();
		RequestLogin req = new RequestLogin(chatOps.LOGIN, email, userName);
		SendRequest(req);
		Start();
	}
	public void CreateNewGroup() {
		Scanner scanner = new Scanner(System.in);
		
		String groupName;
		System.out.println("Please enter new group's name:");
		groupName = scanner.nextLine();
		RequestCreateGroup req = new RequestCreateGroup(chatOps.CREATE_GROUP, userId, groupName);
		SendRequest(req);
	}
	public void JoinGroup() {
		Scanner scanner = new Scanner(System.in);
		
		String groupName;
		System.out.println("Please enter the name of the group you want to join:");
		groupName = scanner.nextLine();
		RequestJoinGroup req = new RequestJoinGroup(chatOps.JOIN_GROUP, userId, groupName);
		SendRequest(req);
	}
	public void LeaveGroup() {
		Scanner scanner = new Scanner(System.in);
		
		String groupName;
		System.out.println("Please enter the name of the group you want to leave:");
		groupName = scanner.nextLine();
		RequestLeaveGroup req = new RequestLeaveGroup(chatOps.LEAVE_GROUP, userId, groupName);
		SendRequest(req);
	}
	public void SendMsg() {
		Scanner scanner = new Scanner(System.in);
		
		String groupName;
		String message;
		System.out.println("Please enter the group name:");
		groupName = scanner.nextLine();
		System.out.println("Please enter the message:");
		message = scanner.nextLine();
		RequestSend req = new RequestSend(chatOps.SEND_MSG, userId, groupName, message);
		SendRequest(req);
	}
	
	public void Logout() {
		RequestLogout req = new RequestLogout(chatOps.LOGOUT, userId);
		SendRequest(req);
		loggedIn = false;
		responseThread.interrupt();
	}
	
	public void SendRequest(Request req) {
		ByteArrayOutputStream bos = new ByteArrayOutputStream();
		ObjectOutputStream out = null;
		DataOutputStream outputStream = null;
		byte[] byteArr;
		
		try {
		  out = new ObjectOutputStream(bos);   
		  out.writeObject(req);
		  out.flush();
		  byteArr = bos.toByteArray();
		  outputStream = new DataOutputStream(clientSocket.getOutputStream());
		  outputStream.write(byteArr);
		} catch (IOException e) {
			e.printStackTrace();
		} finally {
		  try {
		    bos.close();
		  } catch (IOException ex) {
		    ex.printStackTrace();
		  }
		}		
	}

	/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */
	/*         	R  E  S  P   O   N   S   E      M  E  T  H  O  D  S                    */
	/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */
	
	public void getResponse() {
		ObjectInputStream ois = null;
		Response response = null;
		while(!responseThread.isInterrupted()) {
			try {
				ois = new ObjectInputStream(new DataInputStream(clientSocket.getInputStream()));
				response = (Response) ois.readObject();
			} catch (IOException | ClassNotFoundException e) {
				continue;
			}						
			if(response.getStatus() != Status.SUCCESS) {
				System.out.println(response.getOpId() + " FAIL: " + response.getStatus());
			} else {
				printResponse[response.getOpId().ordinal()].invokeMethod(response);				
			}
		}
	}
	
	interface ResponseMethod { void invokeMethod(Response res); }
	
	private ResponseMethod[] printResponse = new ResponseMethod[] {
			new ResponseMethod() { public void invokeMethod(Response res) { 
				userId = ((ResponseLogin)res).getUserId();
				System.out.println("You're successfully logged in. Your user ID is " + userId);					
			} },
			new ResponseMethod() { public void invokeMethod(Response res) { 
				userGroups.add(((ResponseCreateGroup)res).getGroupName());
				System.out.println("The group " + ((ResponseCreateGroup)res).getGroupName() + " has been created");
			} },
			new ResponseMethod() { public void invokeMethod(Response res) { 
				if (((ResponseJoinGroup)res).getUserId() == userId) {
					userGroups.add(((ResponseJoinGroup)res).getGroupName());
					System.out.println("You have joined the group: " + ((ResponseJoinGroup)res).getGroupName());			
				} else {
					System.out.println(((ResponseJoinGroup)res).getSenderName() +  " has joined the group: " + ((ResponseJoinGroup)res).getGroupName());										
				}
			} },
			new ResponseMethod() { public void invokeMethod(Response res) { 
				if (((ResponseLeaveGroup)res).getUserId() == userId) {
					userGroups.remove(((ResponseLeaveGroup)res).getGroupName());
					System.out.println("You have left the group " + ((ResponseLeaveGroup)res).getGroupName());					
				} else {
					System.out.println(((ResponseLeaveGroup)res).getSenderName() + " has left the group: " + ((ResponseLeaveGroup)res).getGroupName());		
				}
			} },
			new ResponseMethod() { public void invokeMethod(Response res) { 
				System.out.println(((ResponseSend)res).getGroupName() + " - " + ((ResponseSend)res).getSenderName() + ": " + ((ResponseSend)res).getMsg());					
			} },
			new ResponseMethod() { public void invokeMethod(Response res) { 
				System.out.println("Logged out successfully");	
			} },
	};
	
	public static void main(String[] args) throws UnknownHostException, IOException {
		MyClient2 client = new MyClient2();
	}
}
