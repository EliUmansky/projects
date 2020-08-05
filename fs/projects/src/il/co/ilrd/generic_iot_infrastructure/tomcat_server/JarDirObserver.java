package il.co.ilrd.tomcat_server;

import java.util.function.Consumer;

import il.co.ilrd.tomcat_server.Dispatcher.Callback;

public class JarDirObserver {
	private JarLoader loader;
	private Callback<String> callback;
	
	public JarDirObserver() throws Exception {
		loader = new JarLoader();
		CreateCallback();
	}
	
	private void CreateCallback() throws Exception {
		Consumer<String> loadJar = pathToJar -> loader.load(pathToJar);
		Runnable endOfMonitoring = ()-> { loader.stopMonitoring(); };
		
		callback = new Dispatcher.Callback<>(loadJar, endOfMonitoring);		
	}
	
	public Callback<String> getCallback() {
		return callback;
	}
}
