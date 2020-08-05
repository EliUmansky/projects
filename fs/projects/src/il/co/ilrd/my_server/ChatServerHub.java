package il.co.ilrd.my_server;

import java.awt.Color;
import java.util.HashMap;
import java.util.HashSet;
import java.util.Map;
import java.util.Set;

public class ChatServerHub implements ChatServer{
	
	private Map<String, Integer> mailToId = new HashMap<>();
	private Map<Integer, User> users = new HashMap<>();
	private Map<String, Group> groups = new HashMap<>();
	
	private static class User {
		private static int totalUsers = 0;
		private String name;
		private String email;
		private int ID;
		private Peer peer;
		private Set<String> userGroups = new HashSet<>();

		public User(String name, String email, Peer peer) {
			++totalUsers;
			this.name = name;
			this.email = email;
			this.ID = totalUsers;
			this.peer = peer;
		}	
	}
	
	private static class Group {
		String name;
		Map<Integer, UsrProperties> usrProperties = new HashMap<>();
		Set<User> users = new HashSet<>();
		
		private Group(String name) {
			this.name = name;
		}
	}
	
	public static class ColorUsrProperties implements UsrProperties{
		/**
		 * 
		 */
		private static final long serialVersionUID = 8064877791051713752L;
		private Color color;
		private String name;
		public ColorUsrProperties(Color color) {
			this.color = color;
		}
		@Override
		public Color getColor() {
			return color;
		}
		@Override
		public String getName() {
			return name;
		}
	}
	
	@Override
	public void logIn(int msgID, String email, String name, Peer peer) {
		Status status = Status.SUCCESS;
		Integer userID = mailToId.get(email);
		User user = null;
		if(userID == null) {
			user = new User(name, email, peer);
			users.put(user.ID , user);
			mailToId.put(email, user.ID);
			userID = user.ID;
		}else {
			user = users.get(userID);
			user.peer = peer;
		}
		user.peer.responseLogin(msgID, userID, user.userGroups, status);
	}

	@Override
	public void sendMsg(int msgId, Integer userId, String groupName, String msg) {
		if(userId == null || groupName== null|| msg == null) {
			return;
		}
		Status status = Status.SUCCESS;
		Group group = groups.get(groupName);
		User user = users.get(userId);
		if(group == null) {
			user.peer.responseMessage(msgId, userId, user.name, groupName, new ColorUsrProperties(Color.BLACK), msg, Status.GROUP_NOT_FOUND);
		}else if(!group.users.contains(user)) {
			user.peer.responseMessage(msgId, userId, user.name, groupName, new ColorUsrProperties(Color.BLACK), msg, Status.NOT_IN_GROUP);
		}else {
			UsrProperties propreties = groups.get(groupName).usrProperties.get(userId);
			String name = users.get(userId).name;
			for(User everyUser : groups.get(groupName).users) {
				try {
					everyUser.peer.responseMessage(msgId, userId, name, groupName, propreties, msg, status);
				}catch(NullPointerException e) {
					System.out.println("the peer is null");
				}
			}
		}
	}

	@Override
	public void createNewGroup(int msgId, Integer userId, String groupName) {
		Status status = Status.SUCCESS;
		User user = users.get(userId);
		if(userId == null || groupName == null) {
			return;
		}
		if(groups.get(groupName) != null) {
			status= Status.GROUP_ALREADY_EXISTS;
		}else {
			Group group = new Group(groupName);
			groups.put(groupName,group);
			group.usrProperties.put(userId, new ColorUsrProperties(new Color((int)(Math.random() * 0x1000000))));
			group.users.add(user);
			user.userGroups.add(groupName);
		}
		user.peer.responseCreateGroup(userId, groupName, status);
	}
	
	@Override
	public void joinGroup(int msgId, Integer userId, String groupName) {
		if(userId == null || groupName == null) {
			return;
		}
		User user = users.get(userId);
		Group group = groups.get(groupName);
		if(group == null) {
			user.peer.responseJoinGroup(msgId, userId, user.name, groupName, Status.GROUP_NOT_FOUND);
		}else if(user.userGroups.contains(groupName)) {
			user.peer.responseJoinGroup(msgId, userId, user.name, groupName, Status.ALREADY_IN_GROUP);
		}else {
		
			group.usrProperties.put(userId, new ColorUsrProperties(new Color((int)(Math.random() * 0x1000000))));
			group.users.add(user);
			user.userGroups.add(groupName);
			
			for(User everyUser : group.users) {
				everyUser.peer.responseJoinGroup(msgId, userId, user.name, groupName, Status.SUCCESS);
			}
		}
	}

	@Override
	public void leaveGroup(int msgId, Integer userId, String groupName) {
		if(userId == null || groupName == null) {
			return;
		}
		User user = users.get(userId);
		Group group = groups.get(groupName);
		if(group == null) {
			user.peer.responseLeaveGroup(msgId, userId, groupName, user.name, Status.GROUP_NOT_FOUND);
		}else if(!user.userGroups.contains(groupName)) {
			user.peer.responseLeaveGroup(msgId, userId, groupName, user.name, Status.NOT_IN_GROUP);
		}else {
			
			for(User everyUser : group.users) {
				everyUser.peer.responseLeaveGroup(msgId, userId, groupName, user.name, Status.SUCCESS);
			}	
			group.usrProperties.remove(userId);
			group.users.remove(user);
			user.userGroups.remove(groupName);
		}
	}

	@Override
	public void logOut(int msgID, Integer userId) {
		System.out.println("server logout");
		if(userId == null) {
			return;
		}
		User user = users.get(userId);
		user.peer.responseLogout(msgID, Status.SUCCESS);
		user.peer = null;
		}
}
