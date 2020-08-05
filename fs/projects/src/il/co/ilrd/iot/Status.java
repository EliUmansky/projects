package il.co.ilrd.tomcat_server;

public enum Status {
	
	SUCCESS(200),
	UNAUTHORIZED(401),
	USER_NOT_FOUND(404),
	PRODUCT_NOT_FOUND(404),
	WRONG_PASSWORD(400),
	EMAIL_EXISTS(422),
	WRONG_INPUT(400);

	private final int status;
	
	private Status(int status) {
		this.status = status;
	}

	public int getStatus() {
		return status;
	}
}
