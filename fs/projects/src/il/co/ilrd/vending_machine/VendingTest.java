package il.co.ilrd.vending_machine;


public class VendingTest {

	public static void main(String[] args) {
		PrintableImp monitor = new PrintableImp();
		int[] stocks1 = {5, 5, 2, 4, 3, 4};
		int[] stocks2 = {1, 1, 1, 1, 1, 1};
		VendingMachine test1 = new VendingMachine(monitor, stocks1);
		VendingMachine test2 = new VendingMachine(monitor, stocks2);

		test1.order(15); // The price of SPRITE is: 4.5 NIS
		test1.insertCoin(2); // Your balance is: 2.0 NIS
		test1.order(15); // Not enough money inserted, the item price is: 4.5 NIS
		test2.insertCoin(3); // Your balance is: 3.0 NIS
		test1.order(19); // Wrong item number
		test1.insertCoin(5); // Your balance is: 7.0 NIS
		test1.order(15); // Enjoy your sprite! Here's your change, 2.5 NIS
		test2.order(11); // Enjoy your tutit! Here's your change, 1.0 NIS
		test1.insertCoin(5); // Your balance is: 5.0 NIS
		test1.order(11); // Enjoy your tutit! Here's your change, 3.0 NIS
		test2.insertCoin(5); // Your balance is: 5.0 NIS
		test2.order(11); // Sorry, the tutit is out of stock
		test2.returnChange(); // Here's your change, 5.0 NIS
		test1.order(14); // The price of WATER is: 3.5 NIS
		test1.insertCoin(5); // Your balance is: 5.0 NIS
		test1.order(15); // Enjoy your sprite! Here's your change, 0.5 NIS
		test1.insertCoin(5); // Your balance is: 5.0 NIS
		test1.order(15); // Enjoy your sprite! Here's your change, 0.5 NIS
		test1.insertCoin(5); // Your balance is: 5.0 NIS
		test1.order(15); // Enjoy your sprite! Here's your change, 0.5 NIS
		test1.insertCoin(5); // Your balance is: 5.0 NIS
		test1.order(15); // Sorry, the sprite is out of stock
		test1.order(12); // Enjoy your zero! Here's your change, 5.5 NIS
	}

}
