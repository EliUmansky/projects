package il.co.ilrd.tomcat_server;

import java.io.IOException;

import javax.servlet.ServletException;
import javax.servlet.annotation.WebServlet;
import javax.servlet.http.HttpServlet;
import javax.servlet.http.HttpServletRequest;
import javax.servlet.http.HttpServletResponse;

import com.google.gson.JsonObject;

/**
 * Servlet implementation class TomcatServer
 */

@WebServlet("/Login")
public class Login extends HttpServlet {
	CrudDBLogin crud;

	private static final long serialVersionUID = 1L;
    /**
     * @see HttpServlet#HttpServlet()
     */
    public Login() {
		try {
			Class.forName("com.mysql.cj.jdbc.Driver");
			crud = new CrudDBLogin("jdbc:mysql://localhost:3306/companies", "root", "ct,h kvmkhj");
		} catch (IOException | ClassNotFoundException e) {
			e.printStackTrace();
		}
    }

	/**
	 * @see HttpServlet#doPost(HttpServletRequest request, HttpServletResponse response)
	 */
	protected void doPost(HttpServletRequest request, HttpServletResponse response) throws ServletException, IOException {
		JsonObject json = HttpRequestToJson.parse(request);
		if (null == json) { 
			sendResponse(response, Status.WRONG_INPUT);
			return;
		}
		String receivedEmail = json.get("email").getAsString();
		try {
			if (crud.read(receivedEmail).equals(Encriptor.encript(json.get("password").getAsString()))) {		
				sendResponse(response, TokenManager.generateToken(receivedEmail));	
			} else {
				sendResponse(response, Status.WRONG_PASSWORD);
			}			
		} catch (NullPointerException e) {
			sendResponse(response, Status.WRONG_INPUT);
		}
	}
	
	private void sendResponse(HttpServletResponse response, Status status) throws IOException {
		response.setContentType("text/html");
		response.setStatus(status.getStatus());
		response.getWriter().println(status);
	}
	
	private void sendResponse(HttpServletResponse response, String token) throws IOException {
		response.setContentType("text/html");
		response.setStatus(Status.SUCCESS.getStatus());
		response.getWriter().println(token);
	}
}

