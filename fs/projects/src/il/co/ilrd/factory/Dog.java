package il.co.ilrd.factory;

public class Dog extends Animal{
	
	public Dog(Args args) {
		super(args);
	}
	
	public Dog() {
		super();
	}
	
	public static Dog getInstance(Args args) {
		return new Dog(args);
	}
	
	@Override
	public String sound() {		
		return "Woof!";
	}
}
