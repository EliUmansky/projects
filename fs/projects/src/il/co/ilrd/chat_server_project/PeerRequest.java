package il.co.ilrd.chat_server_project;

public interface PeerRequest {
	public void requestMessage(int userId, String groupName, String msg);
	public void requestJoinGroup(int userId, String groupName);
	public void requestLogin(String email, String userName);
	public void requestCreateGroup(int userId, String groupName);
	public void requestLeaveGroup(int userId, String groupName);
	public void requestLogout(int userId);
}
