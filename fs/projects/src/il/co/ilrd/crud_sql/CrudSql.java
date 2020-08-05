package il.co.ilrd.crud_sql;

import java.io.IOException;
import java.net.InetAddress;
import java.net.UnknownHostException;
import java.sql.Connection;
import java.sql.DriverManager;
import java.sql.ResultSet;
import java.sql.SQLException;
import java.sql.Statement;

import il.co.ilrd.crud.GenericCrud;

public class CrudSql implements GenericCrud<Integer, String> {
	
	private Connection conn = null;

	public CrudSql(String url, String usr, String password) throws IOException {
		try{  
			conn = DriverManager.getConnection(url, usr, password);  
			
		} catch(Exception e) {
			e.printStackTrace();
		}
	}
	
	@Override
	public void close() throws Exception {
		conn.close();
	}

	@Override
	public Integer create(String data) {
	
		String station_id = "0";
		String insertDataQuery = "insert into Monitor (station_id, data) values( '" + station_id + "', '" + data + "')";
		String getIdQuery = "select MAX(id) from Monitor where station_id = " + station_id;
		Statement insertDataStmt = null;
		Statement getIdStmt = null;
		ResultSet rs = null;
		Integer id = null;
		
		try {
			conn.setAutoCommit(false);	
			insertDataStmt = conn.createStatement();
			insertDataStmt.executeUpdate(insertDataQuery);
			getIdStmt = conn.createStatement();
			rs = getIdStmt.executeQuery(getIdQuery);
			rs.next();
			id = rs.getInt("max(id)");
			conn.commit();
		} catch (SQLException e) {
			e.printStackTrace();
			try {
				conn.rollback();
			} catch (SQLException e1) {
				e1.printStackTrace();
			}
			return null;
		} finally {
			try {
				rs.close();
				insertDataStmt.close();
				getIdStmt.close();
			} catch (SQLException e) {
				e.printStackTrace();
			}
		}
		
		return id;
	}

	@Override
	public String read(Integer key) {
		throw new UnsupportedOperationException();
	}

	@Override
	public void update(Integer key, String obj) {
		throw new UnsupportedOperationException();
	}

	@Override
	public String delete(Integer key) {
		throw new UnsupportedOperationException();
	}
	
	public static void main(String[] args) {
		CrudSql crud = null;
		try {
			crud = new CrudSql("jdbc:mysql://localhost:3306/infinity", "root", "ct,h kvmkhj");
		} catch (IOException e) {
			e.printStackTrace();
		}
		try {
			System.out.println(InetAddress.getLocalHost().getHostAddress());
			System.out.println(InetAddress.getLocalHost().toString());
		} catch (UnknownHostException e1) {
			e1.printStackTrace();
		}
		try {
			crud.close();
		} catch (Exception e) {
			e.printStackTrace();
		}
		
	}

}
