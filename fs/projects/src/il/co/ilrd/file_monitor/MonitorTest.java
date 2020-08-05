package il.co.ilrd.crud;

public class MonitorTest {

	public static void main(String[] args) throws Exception {
		String monitored = "C:\\Users\\Eli\\Documents\\monitored.txt";
		String copyFile = "C:\\Users\\Eli\\Documents\\CopyFile.txt";
		
		FileObserver obs = new FileObserver(copyFile);
		FileMonitor monitor = new FileMonitor(monitored);
		
		monitor.register(obs);
		monitor.startMonitoring();
		
		Thread.sleep(14000);

		monitor.stopMonitoring();
	}

}
