package il.co.ilrd.tomcat_server;

import java.io.IOException;
import java.sql.Connection;
import java.sql.DriverManager;
import java.sql.ResultSet;
import java.sql.SQLException;
import java.sql.Statement;

import com.google.gson.JsonObject;

public class CrudDBProducts {
	private Connection conn = null;

	public CrudDBProducts(String url, String usr, String password) throws IOException {
		try{  		
			conn = DriverManager.getConnection(url, usr, password);  	
		} catch(Exception e) {
			e.printStackTrace();
		}
	}
	
	public void close() throws Exception {
		conn.close();
	}

	public JsonObject read(String email) {
		String getDataQuery = "SELECT * FROM products WHERE email = '" + email + "'";

		try (Statement getDataStmt = conn.createStatement();
				ResultSet rs = getDataStmt.executeQuery(getDataQuery);) {
			return ResultSetToJsonConverter.parseProducts(rs);
		} catch (SQLException e) {
			return null;
		}
	}

	public Status create(String email, ProductDetails details) {
		String insertDataQuery = "INSERT INTO products (email, product_name)"
				+ " VALUES ('" + email + "', '" + details.getProductName() + "')";
		return executeQuery(insertDataQuery, Status.USER_NOT_FOUND, Status.WRONG_INPUT);
	}

	public Status update(String email, ProductDetails details) {	
		String updateDataQuery = "UPDATE products SET product_name ='" + details.getProductName() + 
								 "' WHERE email = '" + email + "' AND product_id = " + details.getProductId();
		return executeQuery(updateDataQuery, Status.PRODUCT_NOT_FOUND, Status.WRONG_INPUT);
	}

	public Status delete(String email, ProductDetails details) {	
		String deleteDataQuery = "DELETE FROM products WHERE email = '" + email + "' and product_id = " + details.getProductId();
		return executeQuery(deleteDataQuery, Status.PRODUCT_NOT_FOUND, Status.USER_NOT_FOUND);
	}
	
	public Status deleteAll(String email) {	
		String deleteAllQuery = "DELETE FROM products WHERE email = '" + email + "'";
		return executeQuery(deleteAllQuery, Status.SUCCESS, Status.WRONG_INPUT);
	}
	
	private Status executeQuery(String query, Status StatusIfNothingChanged, Status StatusIfException) {
		try (Statement Stmt = conn.createStatement();){	
			int numOfLinesChanged = Stmt.executeUpdate(query);
			if (0 == numOfLinesChanged) { return StatusIfNothingChanged; }
		} catch (SQLException e) {
			return StatusIfException;
		} 
		
		return Status.SUCCESS;
	}
	
}