package il.co.ilrd.factory;

public class Args {
	String color;
	int numMasters;
	boolean isHungry;

	public Args(String color, int numMasters, boolean isHungry) {
		this.color = color;
		this.numMasters = numMasters;
		this.isHungry = isHungry;
	}
	
	public Animal getAnimal() {
		return new Animal(this);
	}
	
	public Animal getDog() {
		return new Dog(this);
	}
	
	public Animal getCat() {
		return new Cat(this);
	}
}
