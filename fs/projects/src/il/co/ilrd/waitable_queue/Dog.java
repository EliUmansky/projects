package il.co.ilrd.waitable_queue;

public class Dog {
	private String name;
	private Integer age;
	private String color;
	
	public Dog(String name, Integer age, String color) {
		this.name = name;
		this.age = age;
		this.color = color;
	}

	public String getName() {
		return name;
	}

	public Integer getAge() {
		return age;
	}

	public String getColor() {
		return color;
	}
	
	public boolean isPermutation(String str1, String str2) {
		if (str1.length() != str2.length()) { return false; }
		   int mainIndex = str1.indexOf(str2.charAt(0));
		   
		   for (int index = 1; index < str1.length(); ++index) {
			  int prevInd = -1;
			  int ind = str1.indexOf(str2.charAt(index));

			  if (0 > prevInd) {
				continue;
				 /* if (1 != ind - prevInd) {
					  return str1.equals(str2);
				  }*/
			  } else {
				  if (1 != ind - prevInd) {
					  String str3 = str1.substring(0, mainIndex + 1);
					  return str1.contains(str3);
					  
				  }
			  }
			  
			  prevInd = ind;
		   }
		return false;
	}
}
