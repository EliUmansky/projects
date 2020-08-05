package il.co.ilrd.observer;

public class Kid {
	private int age;
	private int height;
	private double weight;
	
	public Kid(int age, int height, double weight) {
		this.age = age;
		this.height = height;
		this.weight = weight;
	}
	
	public void SetKid(int age, int height, double weight) {
		this.age = age;
		this.height = height;
		this.weight = weight;
	}

	public int getAge() {
		return age;
	}

	public int getHeight() {
		return height;
	}

	public double getWeight() {
		return weight;
	}
}