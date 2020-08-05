package il.co.ilrd.tomcat_server;

import java.io.IOException;
import java.sql.Connection;
import java.sql.DriverManager;
import java.sql.ResultSet;
import java.sql.SQLException;
import java.sql.Statement;

import com.google.gson.JsonObject;

public class CrudDBCompanies {
	
	private Connection conn = null;

	public CrudDBCompanies(String url, String usr, String password) throws IOException {
		try{  		
			conn = DriverManager.getConnection(url, usr, password);  		
		} catch(Exception e) {
			e.printStackTrace();
		}
	}
	
	public void close() throws Exception {
		conn.close();
	}

	public CompanyDetails read(String email) {
		String getDataQuery = "SELECT * FROM companies WHERE email = '" + email + "'";
		JsonObject json = null;
		
		try (Statement getDataStmt = conn.createStatement();
			 ResultSet rs = getDataStmt.executeQuery(getDataQuery);){
			json = ResultSetToJsonConverter.parseCompanies(rs);
		} catch (SQLException e) {
			return null;
		}
		
		return CompanyDetails.getCompanyDetails(json);
	}

	public Status create(CompanyDetails details) {
		String insertDataQuery = "INSERT INTO companies (company_name, email, password, address)"
								+ " VALUES ('" + details.getCompanyName() + "', '" + details.getEmail() + 
								"', '" + details.getEncriptedPassword() + "', '" + details.getAddress() + "')";

		return executeQuery(insertDataQuery, Status.WRONG_INPUT, Status.EMAIL_EXISTS);
	}

	public Status update(String email, CompanyDetails details) {	
		String updateDataQuery = "UPDATE companies SET company_name='" + details.getCompanyName() + "',password='"
							  + details.getEncriptedPassword() + "',address='" + details.getAddress() + "' WHERE email = '" + email + "'";
		
		return executeQuery(updateDataQuery, Status.USER_NOT_FOUND, Status.WRONG_INPUT);
	}

	public Status delete(String email) {
		String deleteDataQuery = "DELETE FROM companies WHERE email = '" + email + "'";
		
		return executeQuery(deleteDataQuery, Status.USER_NOT_FOUND, Status.USER_NOT_FOUND);
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
