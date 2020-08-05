package il.co.ilrd.chat_server_project;

import java.io.Serializable;
import java.util.Collection;
import java.util.List;

class Msg implements Serializable {
	/**
	 * 
	 */
	private static final long serialVersionUID = -9094319427169171676L;
	private static int msgIdCounter = 0;
	private int msgID;
	private chatOps OpId;

	public Msg(chatOps opId) {
		msgID = ++msgIdCounter;
		this.OpId = opId;
	}
	
	public Msg(chatOps opId, int requestMsgID) {
	   	this.msgID = requestMsgID;
		this.OpId = opId;
	}
	
	public int getMsgID() {
		return msgID;
	}

	public chatOps getOpId() {
		return OpId;
	}	
}
class Request extends Msg{
    /**
	 * 
	 */
	private static final long serialVersionUID = 5648428559032414178L;

	public Request(chatOps opId) {
        super(opId);
    }
}

class Response extends Msg{
    /**
	 * 
	 */
	private static final long serialVersionUID = 2444930137654674891L;
	private Status status;
    
    public Response(chatOps opId, int requestMsgID, Status status) {
        super(opId, requestMsgID);
        this.status = status;
    }
    
    public Status getStatus() {
        return status;
    }
}

class RequestLogin extends Request {
	/**
	 * 
	 */
	private static final long serialVersionUID = -2309362424242713828L;
	private String email;
	private String userName;
	
	public RequestLogin(chatOps opId, String email, String userName) {
		super(opId);
		this.email = email;
		this.userName = userName;
	}
	
	public String getEmail() {
		return email;
	}

	public String getUserName() {
		return userName;
	}
}
	
class RequestJoinGroup extends Request {
	/**
	 * 
	 */
	private static final long serialVersionUID = -6996446904646899058L;
	private int userId;
	private String groupName;
	
	public RequestJoinGroup(chatOps opId, int userId, String groupName) {
		super(opId);
		this.userId = userId;
		this.groupName = groupName;
	}
	
	public int getUserId() {
		return userId;
	}

	public String getGroupName() {
		return groupName;
	}
}

class RequestCreateGroup extends Request {
	/**
	 * 
	 */
	private static final long serialVersionUID = -63878996156234175L;
	private int userId;
	private String groupName;
	
	public RequestCreateGroup(chatOps opId, int userId, String groupName) {
		super(opId);
		this.userId = userId;
		this.groupName = groupName;
	}
	
	public int getUserId() {
		return userId;
	}
	
	public String getGroupName() {
		return groupName;
	}
}

class RequestLeaveGroup extends Request {
	/**
	 * 
	 */
	private static final long serialVersionUID = -2400967658850667596L;
	private int userId;
	private String groupName;
	
	public RequestLeaveGroup(chatOps opId, int userId, String groupName) {
		super(opId);
		this.userId = userId;
		this.groupName = groupName;
	}
	
	public int getUserId() {
		return userId;
	}
	
	public String getGroupName() {
		return groupName;
	}
}

class RequestSend extends Request {
	/**
	 * 
	 */
	private static final long serialVersionUID = -4038599863487136690L;
	private int userId;
	private String groupName;
	private String msg;
	
	public RequestSend(chatOps opId, int userId, String groupName, String msg) {
		super(opId);
		this.userId = userId;
		this.groupName = groupName;
		this.msg = msg;
	}

	public int getUserId() {
		return userId;
	}

	public String getGroupName() {
		return groupName;
	}

	public String getMsg() {
		return msg;
	}
}

class RequestLogout extends Request {
	/**
	 * 
	 */
	private static final long serialVersionUID = -4038599863487136690L;
	private int userId;
	
	public RequestLogout(chatOps opId, int userId) {
		super(opId);
		this.userId = userId;
	}

	public int getUserId() {
		return userId;
	}
}

class ResponseLogin extends Response {
	/**
	 * 
	 */
	private static final long serialVersionUID = -5773082868149152439L;
	private int userId;
	private Collection<String> groups;
	
	public ResponseLogin(chatOps opId, int requestMsgID, Status status, int userId, Collection<String> groups) {
		super(opId, requestMsgID, status);
		this.userId = userId;
		this.groups = groups;
	}

	public int getUserId() {
		return userId;
	}

	public Collection<String> getGroups() {
		return groups;
	}
}

class ResponseJoinGroup extends Response {
	/**
	 * 
	 */
	private static final long serialVersionUID = 740161959953103624L;
	private int userId;
	private String groupName;
	private String senderName;
	
	public ResponseJoinGroup(chatOps opId, int requestMsgID, Status status, int userId, String groupName, String senderName) {
		super(opId, requestMsgID, status);
		this.userId = userId;
		this.groupName = groupName;
		this.senderName = senderName;
	}

	public int getUserId() {
		return userId;
	}

	public String getGroupName() {
		return groupName;
	}
	public String getSenderName() {
	    return senderName;
	}
}

class ResponseCreateGroup extends Response {
	/**
	 * 
	 */
	private static final long serialVersionUID = -838016303129502199L;
	private String groupName;
	
	public ResponseCreateGroup(chatOps opId, int requestMsgID, Status status, String groupName) {
		super(opId, requestMsgID, status);
		this.groupName = groupName;
	}


	public String getGroupName() {
		return groupName;
	}
}

class ResponseLeaveGroup extends Response {
	/**
	 * 
	 */
	private static final long serialVersionUID = -246542286706772128L;
	private int userId;
	private String groupName;
	private String senderName;
	
	public ResponseLeaveGroup(chatOps opId, int requestMsgID, Status status, int userId, String groupName, String senderName) {
		super(opId, requestMsgID, status);
		this.userId = userId;
		this.groupName = groupName;
		this.senderName = senderName;
	}

	public int getUserId() {
		return userId;
	}

	public String getGroupName() {
		return groupName;
	}
	
	public String getSenderName() {
	    return senderName;
	}
} 

class ResponseSend extends Response {
	
	/**
	 * 
	 */
	private static final long serialVersionUID = -8256573155482333074L;
	private int userId;
	private String groupName;
	private String msg;
	private String senderName;
	private UsrProperties prop;
	
	public ResponseSend(chatOps opId, int requestMsgID, Status status, int userId, String groupName, String msg, String senderName, UsrProperties prop) {
		super(opId, requestMsgID, status);
		this.userId = userId;
		this.groupName = groupName;
		this.msg = msg;
		this.senderName = senderName;
		this.prop = prop;
	}
	public int getUserId() {
		return userId;
	}
	public String getGroupName() {
		return groupName;
	}
	public String getMsg() {
		return msg;
	}
	public String getSenderName() {
		return senderName;
	}
	public UsrProperties getProp() {
	    return prop;
	}
}

class ResponseLogout extends Response {
	
	/**
	 * 
	 */
	private static final long serialVersionUID = 114386064050502985L;
	
	private int userId;
	
	public ResponseLogout(chatOps opId, int requestMsgID, Status status, int userId) {
		super(opId, requestMsgID, status);
		this.userId = userId;
	}

	public int getUserId() {
		return userId;
	}
} 

