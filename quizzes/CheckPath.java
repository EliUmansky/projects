import java.nio.file.Path;
import java.nio.file.Paths;

public class CheckPath {

	public static void main(String[] args) {
		Path path = Paths.get("/home/student/git/fs/Division/src/CheckPath.java"); 
		String str = path.toString();
		System.out.println(str.endsWith(".java"));
		System.out.println(path.endsWith("*.java"));
        
    } 

}
