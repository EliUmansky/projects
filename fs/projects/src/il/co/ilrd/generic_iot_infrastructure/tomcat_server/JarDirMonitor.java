package il.co.ilrd.tomcat_server;

import static java.nio.file.StandardWatchEventKinds.ENTRY_CREATE;

import java.io.IOException;
import java.nio.file.FileSystems;
import java.nio.file.Path;
import java.nio.file.Paths;
import java.nio.file.WatchEvent;
import java.nio.file.WatchKey;
import java.nio.file.WatchService;
import java.util.List;

public class JarDirMonitor {
	private String monitoredDir;
	private Dispatcher<String> dispatcher;
	private JarDirObserver obs;
	private WatchService watcher;
	private WatchKey watchKey;
	private Path dir;
	
	public JarDirMonitor(String monitoredDir) throws Exception{
		this.monitoredDir = monitoredDir;
		dispatcher = new Dispatcher<>();
		obs = new JarDirObserver();
		dispatcher.register(obs.getCallback());
		createWatcher(monitoredDir);
		startMonitoring();
	}
	
	private void createWatcher(String monitoredDir) throws IOException {
		watcher = FileSystems.getDefault().newWatchService();
		dir = Paths.get(monitoredDir);
		watchKey = dir.register(watcher, ENTRY_CREATE);
	}
	
	public void register(JarDirObserver obs) {
		dispatcher.register(obs.getCallback());
	}

	public void startMonitoring() {
		new Thread(new DirCheck()).start();
	}

	private class DirCheck implements Runnable {
		
		@Override
		public void run() {	
			List<String> jarFiles = FileIterator.iterateDir(monitoredDir);
			
			for (String jarPath : jarFiles) {
				dispatcher.updateAll(jarPath);
			}
			try {
				while (true) {	
					watchKey = watcher.take();
			        for (WatchEvent<?> event : watchKey.pollEvents()) {
			            Path changed = (Path) event.context();
			            if (changed.toString().endsWith(".jar")) {
			            	dispatcher.updateAll(monitoredDir + "/" + changed.toString());	
			            }
			        }
			        
			        if (!watchKey.reset()) { break; }
				}	
			} catch (InterruptedException e) {
				e.printStackTrace();
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
	
	public static void main(String[] args) throws Exception {
		JarDirMonitor monitor = new JarDirMonitor("/home/student/Documents");
	}
}
