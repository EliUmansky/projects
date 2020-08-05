package il.co.ilrd.factory;

public class Cat extends Animal{
	
	public Cat(Args args) {
		super(args);
		this.isHungry = true;
		this.numMasters = 0;
	}
	
	public Cat() {
		super();
		this.isHungry = true;
		this.numMasters = 0;
	}
	
	@Override
	public String sound() {		
		return "meooow!";
	}
}

class CrippledCat extends Cat {
	
	private int numLegs;
	
	public CrippledCat(Args args) {
		super(args);
		this.numLegs = 3;
	}
	
	public CrippledCat() {
		super();
		this.numLegs = 3;
	}
	
	public int getNumLegs() {
		return numLegs;
	}
	
	@Override
	public String sound() {		
		return "*&^%$$%^**!";
	}
}