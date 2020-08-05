package il.co.ilrd.tomcat_server;

import java.io.IOException;
import java.util.function.BiFunction;

import javax.servlet.ServletException;
import javax.servlet.annotation.WebServlet;
import javax.servlet.http.HttpServlet;
import javax.servlet.http.HttpServletRequest;
import javax.servlet.http.HttpServletResponse;

import com.google.gson.JsonObject;

/**
 * Servlet implementation class TomcatServer
 */

@WebServlet("/Products")
public class Products extends HttpServlet {
	CrudDBProducts crud;

	private static final long serialVersionUID = 1L;
    /**
     * @see HttpServlet#HttpServlet()
     */
    public Products() {
		try {
			Class.forName("com.mysql.cj.jdbc.Driver");
			crud = new CrudDBProducts("jdbc:mysql://localhost:3306/companies", "root", "ct,h kvmkhj");
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
		
		JsonObject json = crud.read(email);
		if (null == json) { 
			sendResponse(response, Status.USER_NOT_FOUND);		
			return;
		}

		response.setStatus(Status.SUCCESS.getStatus());
		response.getWriter().println(json);
	}

	/**
	 * @see HttpServlet#doPost(HttpServletRequest request, HttpServletResponse response)
	 */
	protected void doPost(HttpServletRequest request, HttpServletResponse response) throws ServletException, IOException {
		getDetailsAndExecuteCrud(request, response, (email, details) -> crud.create(email, details));
	}

	/**
	 * @see HttpServlet#doPut(HttpServletRequest req, HttpServletResponse resp)
	 */
	protected void doPut(HttpServletRequest request, HttpServletResponse response) throws ServletException, IOException {
		getDetailsAndExecuteCrud(request, response, (email, details) -> crud.update(email, details));
	}
	
	/**
	 * @see HttpServlet#doDelete(HttpServletRequest req, HttpServletResponse resp)
	 */
	protected void doDelete(HttpServletRequest request, HttpServletResponse response) throws ServletException, IOException {
		getDetailsAndExecuteCrud(request, response, (email, details) -> crud.delete(email, details));
	}
	
	private void getDetailsAndExecuteCrud(HttpServletRequest request, HttpServletResponse response, BiFunction<String, ProductDetails, Status> func) throws IOException {
		response.setContentType("text/html");
		JsonObject json = HttpRequestToJson.parse(request);
		if (null == json) { 
			sendResponse(response, Status.WRONG_INPUT);
			return; 
		}
		
		ProductDetails details = ProductDetails.getProductDetails(json);
		if (null == details) { 
			sendResponse(response, Status.WRONG_INPUT); 
			return;
		}
		
		String token = request.getHeader("token");
		if (null == token) {
			sendResponse(response, Status.UNAUTHORIZED);		
			return;
		}	
		String email = TokenManager.getEmail(token);
		Status status = func.apply(email, details);
		sendResponse(response, status);
	}
	
	private void sendResponse(HttpServletResponse response, Status status) throws IOException {
		response.setStatus(status.getStatus());
		response.getWriter().println(status);
	}
}
