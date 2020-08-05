package il.co.ilrd.gateway_server;

import java.io.IOException;
import java.util.concurrent.ExecutorService;
import java.util.concurrent.Executors;

import il.co.ilrd.tomcat_server.JarDirMonitor;

public class GatewayServer {
	private ExecutorService threadPool;
	private TaskManagement taskManagement;
	private JarDirMonitor monitor;
	
	public GatewayServer(String ipAddress, int port, int numOfThreads) throws Exception {
		threadPool = Executors.newFixedThreadPool(numOfThreads);
		taskManagement = new TaskManagement(threadPool);
		new HttpCommunication(taskManagement).listen(ipAddress, port);
		monitor = new JarDirMonitor("/home/student/Documents");
		//new TcpCommunication().listen(ipAddress, 8866);
		//new UdpCommunication().listen(ipAddress, 6868);
	}
	
	public static void main(String[] args) throws IOException {
		try {
			GatewayServer server = new GatewayServer("0.0.0.0", 8500, 4);
		} catch (Exception e) {
			e.printStackTrace();
		}	
	}
}
