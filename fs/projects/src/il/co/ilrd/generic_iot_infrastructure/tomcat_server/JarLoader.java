package il.co.ilrd.tomcat_server;

import java.io.IOException;
import java.lang.reflect.Method;
import java.util.Enumeration;
import java.util.function.Function;
import java.util.jar.JarEntry;
import java.util.jar.JarFile;

import com.google.gson.JsonObject;

import il.co.ilrd.gateway_server.Command;
import il.co.ilrd.gateway_server.CommandFactory;

public class JarLoader {
	
	public void load(String jarPath) {
		JarFile jar = null;
		try {
			jar = new JarFile(jarPath);
		} catch (IOException e1) {
			e1.printStackTrace();
		}
		
	    for (Enumeration<JarEntry> entries = jar.entries() ; entries.hasMoreElements() ;) {
	        JarEntry entry = entries.nextElement();
	        String fileName = entry.getName();
	     
	        if (fileName.endsWith(".class")) {
	            String classname = fileName.replace('/', '.').substring(0, fileName.length() - 6);
	            try {
	            	Class<?> cls = Class.forName(classname);
	            	if (Command.class.isAssignableFrom(cls)) {
	            		String methodName = (String) cls.getMethod("getKey").invoke(null);
	            		Method method = cls.getMethod("getCtorReference");
	    				@SuppressWarnings("unchecked")
	    				Function<JsonObject, Command> func = (Function<JsonObject, Command>) method.invoke(null);
	    				CommandFactory.getInstance().add(methodName, func);
	                }
	            } catch (Exception e) {
	            	e.printStackTrace();
	                System.out.println("WARNING: failed to instantiate " + classname + " from " + fileName);
	            }
	        }
	    }
	}
	
	public void stopMonitoring() {
		
	}
}
