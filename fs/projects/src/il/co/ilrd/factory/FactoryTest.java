package il.co.ilrd.factory;

import static org.junit.jupiter.api.Assertions.*;

import java.lang.reflect.Constructor;
import java.lang.reflect.InvocationTargetException;
import org.junit.jupiter.api.Test;

class FactoryTest {

	@Test
	void testPracticeMethodReferences() {
				
		Factory<String, Animal, Args> fact = new Factory<>();
		
		fact.add("Dog", Dog::new);
		fact.add("Cat", Cat::new);
		fact.add("Animal", Animal::new);
		fact.add("CrippledCat", CrippledCat::new);
		fact.add("CAAAT", (Args) -> new Cat(Args));
		fact.add("StaticDog", Dog::getInstance);
		fact.add("DodLambda", (Args) -> Args.getDog());
		
		Animal animal1 = fact.create("Dog", new Args("Purple", 1, false));
		Animal animal2 = fact.create("Cat", new Args("Pink", 5, false));
		Animal animal3 = fact.create("Animal", new Args("Orange", 6, true));
		Animal animal4 = fact.create("DodLambda", new Args("Blue", 2, false));
		Animal animal5 = fact.create("StaticDog", new Args("Yellow", 4, true));
		Animal animal6 = fact.create("CrippledCat", new Args("Black", 0, true));
		Animal animal7 = fact.create("CAAAT", new Args("Silver", 666, true));
		
		assertEquals("Purple", animal1.getColor());
		assertFalse(animal1.isHungry);
		
		assertEquals("Pink", animal2.getColor());
		assertEquals(0, animal2.getNumMasters());
		assertTrue(animal2.isHungry);	
		
		assertEquals("Orange", animal3.getColor());
		assertTrue(animal3.isHungry);	
		
		assertEquals("Blue", animal4.getColor());
		assertEquals(2, animal4.getNumMasters());
		
		assertEquals("Yellow", animal5.getColor());
		assertEquals(4, animal5.getNumMasters());
		
		assertEquals(3, ((CrippledCat)animal6).getNumLegs());
		assertEquals(0, animal6.getNumMasters());
		
		assertEquals("Silver", animal7.getColor());
		assertTrue(animal7.isHungry);
	}

	
	@Test
	void testAnimalMaker() {
		
		Factory<String, Animal, String> fact = new Factory<>();	
		AnimalMaker maker = new AnimalMaker(new Args("Transparent", 24, false));
	
		fact.add("animal", maker::getAnimal);
		Dog dog = (Dog) fact.create("animal", "Dog");
		Cat cat = (Cat) fact.create("animal", "Cat");
		CrippledCat cripple = (CrippledCat) fact.create("animal", "Crippled Cat");
		
		assertEquals("Woof!", dog.sound());
		assertEquals("meooow!", cat.sound());
		assertEquals("*&^%$$%^**!", cripple.sound());
	}
	
	@Test
	void testReflection() {
		
		Dog[] animals = new Dog[1000000];
		Dog[] animalsRef = new Dog[1000000];
		
		long startTime = System.currentTimeMillis();
		
		for (int i = 0; i < 1000000; ++i) {
			animals[i] = new Dog();			
		}
	
		long endTime = System.currentTimeMillis();
		long execTime = endTime - startTime;
		System.out.println("Execution time: " + execTime);
		

		Constructor<?>[] ctors = animals[0].getClass().getDeclaredConstructors();
		Constructor<?> ctor = null;
		
		for (int i = 0; i < ctors.length; ++i) {
			ctor = ctors[i];
			if (0 == ctor.getGenericParameterTypes().length) { break; }
		}
		
		startTime = System.currentTimeMillis();
		
		for (int i = 0; i < 1000000; ++i) {
			try {
				animalsRef[i] = (Dog) ctor.newInstance();
			} catch (InstantiationException | IllegalAccessException | IllegalArgumentException
					| InvocationTargetException | SecurityException e) {
				e.printStackTrace();
			}
		}
		
		endTime = System.currentTimeMillis();
		execTime = endTime - startTime;
		System.out.println("Execution time with reflection: " + execTime);
		
	}
	
	@Test
	void testArbitrary() {
		
		Factory<String, String, String> fact = new Factory<>();
		
		fact.add("lowerStr", String::toUpperCase);
		String upperStr = fact.create("lowerStr", "ma kore");
		
		assertEquals("MA KORE", upperStr);		
	}
}
