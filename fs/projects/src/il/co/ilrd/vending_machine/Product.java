package il.co.ilrd.vending_machine;


public enum Product {
	COKE(10, 4.5, 0),
	TUTIT(11, 2, 1),
	ZERO(12, 4.5, 2),
	TEA(13, 6.5, 3),
	WATER(14, 3.5, 4),
	SPRITE(15, 4.5, 5)
	;
	
	private int key;
	private double price;
	private int stockIndex;
	
	private Product(int key, double price, int stockIndex) {
		this.key = key;
		this.price = price;
		this.stockIndex = stockIndex;
	}

	public int getKey() {return key;}
	public void setKey(int key) {this.key = key;}
	public double getPrice() {return price;}
	public void setPrice(double price) {this.price = price;}
	public int getStockIndex() {return this.stockIndex;}
}
