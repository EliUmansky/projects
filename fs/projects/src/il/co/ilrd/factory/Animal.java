package il.co.ilrd.factory;

public class Animal {
	public String color;
	public int numMasters;
	public boolean isHungry;
	
	public Animal (Args args) {
		this.color = args.color;
		this.numMasters = args.numMasters;
		this.isHungry = args.isHungry;
	}
	
	public Animal () {
		this.color = "Brown";
		this.numMasters = 2;
		this.isHungry = false;
	}
	
	public String sound() {
		return "Animal sound";
	}
	
	public String getColor() {
		return color;
	}
	
	public boolean getIsHungry() {
		return isHungry;
	}
	
	public int getNumMasters() {
		return numMasters;
	}
}
