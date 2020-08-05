package il.co.ilrd.my_server;

import java.util.Collection;
import java.util.HashSet;
import java.util.Scanner;
import java.util.Set;

public class Client {
	private int userID;
	private String name;
	private String email;
	private Set<String> groupNames = new HashSet<>();
	
	public Client()  {
		Scanner scanner = new Scanner(System.in);
		System.out.println("enter your name");
		name = scanner.nextLine();
		System.out.println("enter your email");
		email = scanner.nextLine();
	}
	
	public void loginResponse(int userID, Collection<String> groupNames) {
		System.out.println("client login ");
		this.userID = userID;
		this.groupNames.addAll(groupNames);
	}
	
	public void createGroupResponse(String group, Status status) {
		if(status == Status.GROUP_ALREADY_EXISTS) {
			System.out.println("GROUP ALREADY EXISTS");
		}else {
			System.out.println(" you created the group ");
			groupNames.add(group);
		}
	}
	public void joinGroupResponse(int userID, String senderName, String group, Status status) {
		if(status == Status.SUCCESS) {
			if(userID == this.userID) {
				groupNames.add(group);
				System.out.println("you joined the group");
			}else {
				System.out.println(senderName + " has joined " + group + " group");
			}
		}else {
			System.out.println("fail status:" + status);
		}
	}
	public void leaveGroupResponse(int userID, String senderName, String group, Status status) {
		if(Status.SUCCESS == status) {
			if(userID == this.userID) {
				groupNames.remove(group);
			}else {
				System.out.println(senderName+" has left "+ group + " group");
			}
		}else {
			System.out.println("fail status:" + status);
		}
	}
	public void messageResponse(int userID, String senderName, String group, String msg, UsrProperties prop, Status status) {
		if(Status.SUCCESS == status) {
			if(userID == this.userID) {
				//ignore
			}else {
				System.out.println(group+" :");
				System.out.println(prop.getColor());
				System.out.println(senderName + " : " + msg);
			}
		}else {
			System.out.println("fail status:" + status);
		}
	}
	public void logOutResponse(int userID) {
		System.out.println("response loglot final client");
		if(userID == this.userID) {
			System.out.println("need to logout - not yet implemented");
			System.out.println(userID);
			
		}else {
			//ignore
		}
	}
	public String getName() {
		return name;
	}
	public String getEmail() {
		return email;
	}
	public int getUserID() {
		return userID;
	}
	
	
	
	
}
	