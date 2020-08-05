package il.co.ilrd.crud;

import java.io.BufferedWriter;
import java.io.FileWriter;
import java.io.IOException;

public class CrudFile implements GenericCrud<Integer, String> {

	private BufferedWriter bufferedWriter;
	private Integer lineNumber = 0;
	
	public CrudFile(String clientFilePath) throws IOException {
		bufferedWriter = new BufferedWriter(new FileWriter(clientFilePath));
	}
	
	@Override
	public void close() throws Exception {
		bufferedWriter.close();
	}

	@Override
	public Integer create(String line) {
		try {
			bufferedWriter.append(line);
			bufferedWriter.append("\n");
			bufferedWriter.flush();
		} catch (IOException e) {
			System.err.println("There is a problem with the specified file");
			e.printStackTrace();
			try {
				close();
			} catch (Exception e1) {
				System.err.println("There is a problem with closing the stream");
			}
		}
		return ++lineNumber;
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

}
