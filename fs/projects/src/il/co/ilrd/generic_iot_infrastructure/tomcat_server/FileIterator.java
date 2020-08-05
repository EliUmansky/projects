package il.co.ilrd.tomcat_server;

import java.io.File;
import java.util.List;
import java.util.ArrayList;

public class FileIterator {
	
	public static List<String> iterateDir(String dirPath) {
		File[] files = new File(dirPath).listFiles();
		List<String> jarFiles = new ArrayList<>();
		
		return showFiles(files, jarFiles);
	}
	
	private static List<String> showFiles(File[] files, List<String> jarFiles) {
	    for (File file : files) {
	        if (file.isDirectory()) {
	            showFiles(file.listFiles(), jarFiles);
	        } else {
	            if (file.getName().endsWith(".jar")) {
	            	jarFiles.add(file.getAbsolutePath());	            	
	            }
	            
	        }
	    }
	    
	    return jarFiles;
	}
}
