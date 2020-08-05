package il.co.ilrd.tomcat_server;

import java.io.IOException;
import java.sql.Connection;
import java.sql.DriverManager;
import java.sql.ResultSet;
import java.sql.SQLException;
import java.sql.Statement;

public class CrudDBLogin {

	private Connection conn = null;
	
	public CrudDBLogin(String url, String usr, String password) throws IOException {
		try{  		
			conn = DriverManager.getConnection(url, usr, password);  
			
		} catch(Exception e) {
			e.printStackTrace();
		}
	}
	
	public void close() throws Exception {
		conn.close();
	}

	public String read(String key) {
		String getDataQuery = "SELECT password FROM companies WHERE email = '" + key + "'";
		Statement getDataStmt = null;
		ResultSet rs = null;
		String encriptedPassword = null;
		
		try {
			getDataStmt = conn.createStatement();
			rs = getDataStmt.executeQuery(getDataQuery);
			if (rs.next() == false) {
		        return null;
		    } else { 
		        encriptedPassword = rs.getString("password");
		    }
		} catch (SQLException e) {
			e.printStackTrace();
		} finally {
			try {
				rs.close();
				getDataStmt.close();
			} catch (SQLException e) {
				e.printStackTrace();
			}
		}
		
		return encriptedPassword;
	}

}
