package il.co.ilrd.tomcat_server;

import java.io.IOException;

import javax.servlet.ServletException;
import javax.servlet.annotation.WebServlet;
import javax.servlet.http.HttpServlet;
import javax.servlet.http.HttpServletRequest;
import javax.servlet.http.HttpServletResponse;

import com.google.gson.JsonObject;

import il.co.ilrd.tomcat_server.CrudDBProducts;

/**
 * Servlet implementation class TomcatServer
 */

@WebServlet("/Companies")
public class Companies extends HttpServlet {
	CrudDBCompanies crud;
	CrudDBProducts prodCrud;
	private static final long serialVersionUID = 1L;
    /**
     * @see HttpServlet#HttpServlet()
     */
    public Companies() {
		try {
			Class.forName("com.mysql.cj.jdbc.Driver");
			crud = new CrudDBCompanies("jdbc:mysql://localhost:3306/companies", "root", "ct,h kvmkhj");
			prodCrud = new CrudDBProducts("jdbc:mysql://localhost:3306/companies", "root", "ct,h kvmkhj");
		} catch (IOException | ClassNotFoundException e) {
			e.printStackTrace();
		}
    }

	/**
	 * @see HttpServlet#doGet(HttpServletRequest request, HttpServletResponse response)
	 */
	protected void doGet(HttpServletRequest request, HttpServletResponse response) throws ServletException, IOException {
		response.setContentType("application/json");
		
		String token = request.getHeader("token");
		if (null == token) {
			sendResponse(response, Status.UNAUTHORIZED);		
			return;
		}
		
		String email = TokenManager.getEmail(token);
		if (null == email) {
			sendResponse(response, Status.UNAUTHORIZED);		
			return;
		}
		
		CompanyDetails details = crud.read(email);
		if (null == details) { 
			sendResponse(response, Status.USER_NOT_FOUND);		
			return;
		}
		
		sendResponse(response, details);
	}

	/**
	 * @see HttpServlet#doPost(HttpServletRequest request, HttpServletResponse response)
	 */
	protected void doPost(HttpServletRequest request, HttpServletResponse response) throws ServletException, IOException {
		response.setContentType("text/html");
		JsonObject json = HttpRequestToJson.parse(request);
		if (null == json) { 
			sendResponse(response, Status.WRONG_INPUT);	
			return;
		}
		
		CompanyDetails details = CompanyDetails.getCompanyDetails(json);
		if (null == details) { 
			sendResponse(response, Status.WRONG_INPUT);	
			return;
		}
		
		Status status = crud.create(details);
		
		sendResponse(response, status);
	}

	/**
	 * @see HttpServlet#doPut(HttpServletRequest req, HttpServletResponse resp)
	 */
	protected void doPut(HttpServletRequest request, HttpServletResponse response) throws ServletException, IOException {
		response.setContentType("text/html");
		JsonObject json = HttpRequestToJson.parse(request);
		if (null == json) { 
			sendResponse(response, Status.WRONG_INPUT);	
			return;
		}
		
		String token = request.getHeader("token");
		if (null == token) {
			sendResponse(response, Status.UNAUTHORIZED);		
			return;
		}		
		
		String email = TokenManager.getEmail(token);
		if (null == email) {
			sendResponse(response, Status.UNAUTHORIZED);		
			return;
		}
		json.addProperty("email", email);
	
		CompanyDetails details = CompanyDetails.getCompanyDetails(json);
		if (null == details) { 
			sendResponse(response, Status.WRONG_INPUT);
			return;
		}

		sendResponse(response, crud.update(email, details));
	}
	
	/**
	 * @see HttpServlet#doDelete(HttpServletRequest req, HttpServletResponse resp)
	 */
	protected void doDelete(HttpServletRequest request, HttpServletResponse response) throws ServletException, IOException {
		response.setContentType("text/html");
		String token = request.getHeader("token");
		if (null == token) {
			sendResponse(response, Status.UNAUTHORIZED);		
			return;
		}
		
		String email = TokenManager.getEmail(token);
		if (null == email) {
			sendResponse(response, Status.UNAUTHORIZED);		
			return;
		}
		
		Status deleteProdsStatus = prodCrud.deleteAll(email);
		if (Status.SUCCESS.getStatus() != deleteProdsStatus.getStatus()) {
			sendResponse(response, deleteProdsStatus);
			return;
		}
		sendResponse(response, crud.delete(email));
	}
	
	private void sendResponse(HttpServletResponse response, Status status) throws IOException {
		response.setStatus(status.getStatus());
		response.getWriter().println(status);
	}
	
	private void sendResponse(HttpServletResponse response, CompanyDetails details) throws IOException {
		response.setStatus(Status.SUCCESS.getStatus());
		response.getWriter().println(CompanyDetails.getJson(details));
	}
}

