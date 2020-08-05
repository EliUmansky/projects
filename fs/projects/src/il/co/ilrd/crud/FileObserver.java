package il.co.ilrd.crud;

import java.util.function.Consumer;

import il.co.ilrd.observer.Dispatcher;

public class FileObserver {

	private CrudFile crud;
	private Dispatcher.Callback<String> callback;
	
	public FileObserver(String clientFilePath) throws Exception {
		crud = new CrudFile(clientFilePath);
		CreateCallback();
	}
	
	private void CreateCallback() throws Exception {
		Consumer<String> createNewLine = msg -> crud.create(msg);
		Runnable endOfMonitoring = ()-> {
			try {
				crud.close();
			} catch (Exception e) {
				e.printStackTrace();
			}
		};
		
		callback = new Dispatcher.Callback<>(createNewLine, endOfMonitoring);		
	}
	
	public Dispatcher.Callback<String> getCallback() {
		return callback;
	}
}
