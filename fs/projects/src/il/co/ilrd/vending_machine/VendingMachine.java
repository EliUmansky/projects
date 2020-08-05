package il.co.ilrd.vending_machine;

public class VendingMachine {
	private double balance = 0;
	private State state;
	private int[] stock;
	private final PrintableImp monitor;
	
	VendingMachine(PrintableImp monitor, int[] stock) {
		this.stock = stock;
		this.monitor = monitor;
		state = State.WaitForCoin;
	}
	
	private double getBalance() {return balance;}
	private void ReduceStock(int index) {
		--stock[index];
	}
	public void insertCoin(double coin) {state.gotCoin(this, coin);}
	public void order(int key) {state.gotOrder(this, key);}
	public void returnChange() {
		if (0 < getBalance()) {
			monitor.printStr("Here's your change, " + getBalance() + " NIS");
			balance = 0;			
		}
	}
	
	public enum State {		
		WaitForCoin {
			@Override
			public void gotOrder(VendingMachine machine, int key) {
				Product[] products = Product.values();
				for (Product item : products) {
					if (key == item.getKey()) {
						machine.monitor.printStr("The price of " + item.name() + " is: " + item.getPrice() + " NIS");
						break;
					}
				}
			}
		},
		WaitForOrder {
			@Override
			public void gotOrder(VendingMachine machine, int key) {
				Product[] products = Product.values();
				for (Product item : products) {
					if (key == item.getKey()) {
						if (0 == machine.stock[item.getStockIndex()]) {
							machine.monitor.printStr("Sorry, the " + item.name().toLowerCase() + " is out of stock");
							return;
						}
						
						if (machine.getBalance() < item.getPrice()) {
							machine.monitor.printStr("Not enough money inserted, the item price is: " + item.getPrice() + " NIS");
						} 
						else {
							machine.balance -= item.getPrice();
							machine.ReduceStock(item.getStockIndex());
							machine.monitor.printStr("Enjoy your " + item.name().toLowerCase() + "!");
							machine.returnChange();
							machine.state = WaitForCoin;
						}
						return;
					}
				}
				machine.monitor.printStr("Wrong item number");
			}
		};
		
		public void gotCoin(VendingMachine machine, double coin) {
			machine.balance += coin;
			machine.monitor.printStr("Your balance is: " + machine.getBalance() + " NIS");
			machine.state = WaitForOrder;
		}
		public abstract void gotOrder(VendingMachine machine, int key);
	}
}