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
 
    public Products() {
		try {
			Class.forName("com.mysql.cj.jdbc.Driver");
			crud = new CrudDBProducts("jdbc:mysql://localhost:3306/companies", "root", "ct,h kvmkhj");
		} catch (IOException | ClassNotFoundException e) {
			e.printStackTrace();
		}
    }

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

	protected void doPost(HttpServletRequest request, HttpServletResponse response) throws ServletException, IOException {
		JsonObject json = getDetailsAndExecuteCrud(request, response, (email, details) -> crud.create(email, details));
		if (null != json) {
			JsonObject jsonRequest = new JsonObject();
			jsonRequest.addProperty("key", "PR");
			jsonRequest.add("data", json);
			HttpJsonSending.sendHTTPRequest("http://0.0.0.0:8500", jsonRequest.toString());
		}
	}

	protected void doPut(HttpServletRequest request, HttpServletResponse response) throws ServletException, IOException {
		getDetailsAndExecuteCrud(request, response, (email, details) -> crud.update(email, details));
	}

	protected void doDelete(HttpServletRequest request, HttpServletResponse response) throws ServletException, IOException {
		getDetailsAndExecuteCrud(request, response, (email, details) -> crud.delete(email, details));
	}
	
	private JsonObject getDetailsAndExecuteCrud(HttpServletRequest request, HttpServletResponse response, BiFunction<String, ProductDetails, Status> func) throws IOException {
		response.setContentType("text/html");
		
		ProductDetails details = null;
		JsonObject json = null;
		try {
			json = HttpRequestToJson.parse(request);
			details = ProductDetails.getProductDetails(json);	
		} catch (Exception e) {
			sendResponse(response, Status.WRONG_INPUT);
			return null;
		}
		
		Status st = executeCrud(request, response, func, details);
		sendResponse(response, st);
		
		return json;
	}
	
	private Status executeCrud(HttpServletRequest request, HttpServletResponse response, BiFunction<String, ProductDetails, Status> func, ProductDetails details) {
		try {
			String token = request.getHeader("token");
			String email = TokenManager.getEmail(token);
			Status status = func.apply(email, details);
			
			return status;
		} catch (Exception e) {
			return Status.UNAUTHORIZED;
		}
	}
	
	private void sendResponse(HttpServletResponse response, Status status) throws IOException {
		response.setStatus(status.getStatus());
		response.getWriter().println(status);
	}
}





























