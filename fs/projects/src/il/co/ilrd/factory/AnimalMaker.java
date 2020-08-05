package il.co.ilrd.factory;

public class AnimalMaker {

	public String color;
	public int numMasters;
	public boolean isHungry;
	
	public AnimalMaker(Args args) {
		this.color = args.color;
		this.numMasters = args.numMasters;
		this.isHungry = args.isHungry;
	}

	public Animal getAnimal(String str) {
		switch (str) {
		case "Dog":
			return new Dog();
		case "Cat":
			return new Cat();
		case "Crippled Cat":
			return new CrippledCat();
		default:
			return null;
		}
	}

}