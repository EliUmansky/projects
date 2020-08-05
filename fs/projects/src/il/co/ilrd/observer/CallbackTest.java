package il.co.ilrd.observer;

import java.util.function.Consumer;

public class CallbackTest {

	public static void main(String[] args) {
		Consumer<String> bonobo = msg -> System.out.println("A big " + msg + " escaped from the cage, bonobo is afraid!");
		Runnable notifyBonobo = ()-> System.out.println("Ba-bye, bonobo");
		Consumer<String> lemur = msg -> System.out.println("A big " + msg + " escaped from the cage, lemur is afraid!");
		Runnable notifyLemur = ()-> System.out.println("Ba-bye, lemur");
		Consumer<String> cat = msg -> System.out.println("A big " + msg + " escaped from the cage, cat couldn't care less");
		Runnable notifyCat = ()-> System.out.println("Ba-bye, cat");
		
		Dispatcher<String> zooKeeper = new Dispatcher<>();
		Dispatcher.Callback<String> bonoboCB =  new Dispatcher.Callback<>(bonobo, notifyBonobo);
		Dispatcher.Callback<String> lemurCB =  new Dispatcher.Callback<>(lemur, notifyLemur);
		Dispatcher.Callback<String> catCB =  new Dispatcher.Callback<>(cat, notifyCat);
		
		zooKeeper.register(bonoboCB);
		zooKeeper.register(lemurCB);
		zooKeeper.register(catCB);
		
		zooKeeper.updateAll("tiger");
		System.out.println(" ");
		catCB.stopService();
		
		zooKeeper.updateAll("lion");
		System.out.println(" ");

		zooKeeper.stopAll();
		
		System.out.println("------------------------------------------");
		
		Consumer<Integer> howlerMonkey = msg -> System.out.println(msg + " bananas? Awesome!! Howler monkeys loove bananas");
		Runnable notifyHowlerMonkey = ()-> System.out.println("Oh, nooo! Howler's gonna be hungry");
		Consumer<Integer> spiderMonkey = msg -> System.out.println(msg + " bananas? Awesome!! Spider monkeys loove bananas");
		Runnable notifySpiderMonkey = ()-> System.out.println("Oh, nooo! Spider monkey's gonna be hungry");
		Consumer<Integer> jaguar = msg -> System.out.println("Jaguars don't eat bananas, better gimme " + msg + " capybaras!!");
		Runnable notifyJaguar = ()-> System.out.println("Good riddance");
		
		Dispatcher<Integer> animalChef = new Dispatcher<>();
		
		Dispatcher.Callback<Integer> howlerCB =  new Dispatcher.Callback<>(howlerMonkey, notifyHowlerMonkey);
		Dispatcher.Callback<Integer> spiderCB =  new Dispatcher.Callback<>(spiderMonkey, notifySpiderMonkey);
		Dispatcher.Callback<Integer> jaguarCB =  new Dispatcher.Callback<>(jaguar, notifyJaguar);

		animalChef.register(howlerCB);
		animalChef.register(spiderCB);
		animalChef.register(jaguarCB);
		
		animalChef.updateAll(7);
		System.out.println(" ");
		animalChef.stopAll();
		
		jaguarCB.stopService();
	}

}
