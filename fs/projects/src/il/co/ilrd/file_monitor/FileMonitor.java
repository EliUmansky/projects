package il.co.ilrd.crud;

import java.io.BufferedReader;
import java.io.File;
import java.io.FileReader;
import java.io.IOException;
import java.nio.file.ClosedWatchServiceException;
import java.nio.file.FileSystems;
import java.nio.file.Path;
import java.nio.file.Paths;
import java.nio.file.WatchEvent;
import java.nio.file.WatchKey;
import java.nio.file.WatchService;

import static java.nio.file.StandardWatchEventKinds.*;

import il.co.ilrd.observer.Dispatcher;

public class FileMonitor {
	private String monitoredFilePath;
	private Dispatcher<String> dispatcher;
	private WatchService watcher;
	private WatchKey watchKey;
	private Path dir;
	
	FileMonitor(String monitoredFilePath) throws IOException, InterruptedException {
		this.monitoredFilePath = monitoredFilePath;
		dispatcher = new Dispatcher<>();
		createWatcher(monitoredFilePath);
	}
	
	private void createWatcher(String monitoredFilePath) throws IOException {
		watcher = FileSystems.getDefault().newWatchService();
		dir = Paths.get(monitoredFilePath).getParent();
		watchKey = dir.register(watcher, ENTRY_MODIFY);
	}
	
	public void register(FileObserver obs) {
		dispatcher.register(obs.getCallback());
	}

	public void startMonitoring() {
		new Thread(new FileCheck()).start();
	}

	private class FileCheck implements Runnable {
		
		@Override
		public void run() {			
			String msg = null;
			File monitored = new File(monitoredFilePath);

			try(BufferedReader bufferedReader = new BufferedReader(new FileReader(monitoredFilePath))) {
				while (null != (msg = bufferedReader.readLine())) {
					dispatcher.updateAll(msg);
				}

				while (true) {	
					watchKey = watcher.take();
			        for (WatchEvent<?> event : watchKey.pollEvents()) {
			            Path changed = (Path) event.context();
			            if (changed.endsWith(monitored.getName())) {
			            	dispatcher.updateAll(bufferedReader.readLine());	
			            }
			        }
			        
			        if (!watchKey.reset()) { break; }
				}	
			} catch (IOException | InterruptedException e) {
				e.printStackTrace();
			} catch (ClosedWatchServiceException e) {
				
			} finally { dispatcher.stopAll(); }
		}		
	}
	
	public void stopMonitoring() {
		try {
			watcher.close();
		} catch (IOException e) {
			e.printStackTrace();
		}
	}
}
