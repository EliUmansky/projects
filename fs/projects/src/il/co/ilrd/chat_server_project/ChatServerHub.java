package il.co.ilrd.chat_server_project;

import java.awt.Color;
import java.util.HashMap;
import java.util.HashSet;
import java.util.Map;
import java.util.Set;

public class ChatServerHub implements ChatServer{
	
	private Map<Integer, User> users;
	private Map<String, Group> groups;
	private Map<String, Integer> emailToId;
	
	ChatServerHub() {
		users = new HashMap<>();
		groups = new HashMap<>();
		emailToId = new HashMap<>();
	}
	
	@Override
	public void logIn(int msgId, String email, String name, PeerResponse peer) {
		if (null == email || null == name || null == peer) { 
			return;
		}
		
		Integer userID = emailToId.get(email);
		User newUser;
		if (null == userID) {
			newUser = new User(name, email, peer);
			userID = newUser.id;
			users.put(userID, newUser);
			emailToId.put(email, userID);
		} else {
			newUser = users.get(userID);
			newUser.peer = peer;
		}
		
		peer.responseLogin(msgId, userID, newUser.userGroups, Status.SUCCESS);
	}
	
	@Override
	public void createNewGroup(int msgId, Integer userId, String groupName) {
		if (null == userId || null == groupName) { return; }
		
		if (groups.containsKey(groupName)) {
			users.get(userId).peer.responseCreateGroup(msgId, groupName,  Status.GROUP_ALREADY_EXISTS);
			return;
		} 
		
		Group newGroup = new Group(groupName);
		Color color = new Color((int)(Math.random() * 0x1000000));
		
		newGroup.users.put(userId, new ColorUsrProperties(color));			
		groups.put(groupName, newGroup);
		users.get(userId).userGroups.add(groupName);
		users.get(userId).peer.responseCreateGroup(msgId, groupName,  Status.SUCCESS);
		
	}

	@Override
	public void joinGroup(int msgId, Integer userId, String groupName) {
		if (null == userId || null == groupName) { return; }
		
		Group groupToJoin = groups.get(groupName);
		if (null == groupToJoin) {
			users.get(userId).peer.responseJoinGroup(msgId, userId, users.get(userId).name, groupName, Status.GROUP_NOT_FOUND);
			return;
		} else if (groupToJoin.users.containsKey(userId)) {
			users.get(userId).peer.responseJoinGroup(msgId, userId, users.get(userId).name, groupName, Status.ALREADY_IN_GROUP);
			return;
		}
		
		User user = users.get(userId);
		user.userGroups.add(groupName);
		Color color = new Color((int)(Math.random() * 0x1000000));
		ColorUsrProperties prop = new ColorUsrProperties(color);
		groupToJoin.users.put(userId, prop);
		groups.get(groupName).users.put(userId, prop);
		for (Integer Id : groups.get(groupName).users.keySet()) {
			if (null != users.get(Id).peer) {
				users.get(Id).peer.responseJoinGroup(msgId, userId, users.get(userId).name, groupName, Status.SUCCESS);
			}
		}
	}

	@Override
	public void leaveGroup(int msgId, Integer userId, String groupName) {
		if (null == userId || null == groupName) { return; }
		Group groupToLeave = groups.get(groupName);
		if (null == groupToLeave) {
			users.get(userId).peer.responseLeaveGroup(msgId, userId, users.get(userId).name, groupName, Status.GROUP_NOT_FOUND);
			return;
		} else if (!groupToLeave.users.containsKey(userId)) {
			users.get(userId).peer.responseLeaveGroup(msgId, userId, users.get(userId).name, groupName, Status.NOT_IN_GROUP);
			return;
		}

		for (Integer Id : groups.get(groupName).users.keySet()) {
			if (null != users.get(Id).peer) {
				users.get(Id).peer.responseLeaveGroup(msgId, userId, users.get(userId).name, groupName, Status.SUCCESS);						
			}
		}
		users.get(userId).userGroups.remove(groupName);
		groups.get(groupName).users.remove(userId);
		groupToLeave.users.remove(userId);
	}

	@Override
	public void sendMsg(int msgId, Integer userId, String groupName, String msg) {
		if (null == userId || null == groupName || null == msg) { return; }
		
		Group group = groups.get(groupName);
		if (null == group) {
			users.get(userId).peer.responseMessage(msgId, userId, users.get(userId).name, groupName, new ColorUsrProperties(Color.RED), "Group not found", Status.GROUP_NOT_FOUND);
			return;
		} else if (!group.users.containsKey(userId)) {
			users.get(userId).peer.responseMessage(msgId, userId, users.get(userId).name, groupName, new ColorUsrProperties(Color.RED), "Not in group", Status.NOT_IN_GROUP);
			return;
		}
		
		for (Integer Id : groups.get(groupName).users.keySet()) {
			if (null != users.get(Id).peer) {
				users.get(Id).peer.responseMessage(msgId, userId, users.get(userId).name, groupName, groups.get(groupName).users.get(userId), msg, Status.SUCCESS);							
			}
		}
	}

	@Override
	public void logOut(int msgId, Integer userId) {
		if (null == userId) { return; }
		
		users.get(userId).peer.responseLogout(msgId, userId, Status.SUCCESS);
		users.get(userId).peer = null;
	}
	
	private static class User {
		private String name;
		private String email;
		private int id;
		private PeerResponse peer;
		private static int counter;
		private Set<String> userGroups;
		
		public User(String name, String email, PeerResponse peer) {
			this.name = name;
			this.email = email;
			this.id = ++counter;
			this.peer = peer;
			userGroups = new HashSet<>();
		}	
	}
	
	static class Group {
		private static int groupIdCounter;
		private int groupID;
		private String name;
		Map<Integer, UsrProperties> users;
		
		Group(String name) {
			this.name = name;
			groupID = ++groupIdCounter;
			users = new HashMap<>();
		}
	}
	
	static class ColorUsrProperties implements UsrProperties{
		/**
		 * 
		 */
		private static final long serialVersionUID = -723772885435135941L;
		private Color color;
		
		public ColorUsrProperties(Color color) {
			this.color = color;
		}
		@Override
		public Color getColor() {
			return color;
		}
	}

	
}
