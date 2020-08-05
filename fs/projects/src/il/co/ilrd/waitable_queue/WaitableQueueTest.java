package il.co.ilrd.waitable_queue;

import static org.junit.jupiter.api.Assertions.*;

import org.junit.jupiter.api.Test;
import java.util.Comparator;
import java.util.Random;

class WaitableQueueTest {

	@Test
	void testComparator() {
		Comparator<String> comparator = (String str1, String str2) -> {return str1.compareTo(str2);};
		WaitableQueue<String> queue = new WaitableQueue<>(comparator);
		
		queue.enqueue("abcd");
		queue.enqueue("bcda");
		queue.enqueue("dabc");
		queue.enqueue("cdab");
		
		
		
		assertEquals("abcd", queue.dequeue());
		assertEquals("bcda", queue.dequeue());
		assertEquals("cdab", queue.dequeue());
		assertEquals("dabc", queue.dequeue());
		
	}
/*	
	@Test
	void testComparableTimeout() throws InterruptedException {
	
		WaitableQueue<String> queue = new WaitableQueue<>();
		
		queue.enqueue("abcd");
		queue.enqueue("bcda");
		queue.enqueue("dabc");
		queue.enqueue("cdab");
		
		assertEquals("abcd", queue.dequeue());
		assertEquals("bcda", queue.dequeue());
		assertEquals("cdab", queue.dequeueWithTimeout(3));
		assertEquals("dabc", queue.dequeueWithTimeout(7));
		assertNull(queue.dequeueWithTimeout(2));
		
	}
	
	@Test
	void testDog() {
		Comparator<Dog> comparator = (Dog dog1, Dog dog2) -> dog1.getAge().compareTo(dog2.getAge());
		WaitableQueue<Dog> queue = new WaitableQueue<>(comparator);
		
		Dog dog1 = new Dog("Rex", 7, "Brown");
		Dog dog2 = new Dog("Bob", 4, "Black");
		Dog dog3 = new Dog("Dog", 3, "White");

		queue.enqueue(dog1);
		queue.enqueue(dog2);
		queue.enqueue(dog3);
	
		
		assertEquals("Dog", queue.dequeue().getName());
		assertEquals("Bob", queue.dequeue().getName());
		assertEquals("Brown", queue.dequeue().getColor());
	}

	@Test
	void ThreadTest() {
		WaitableQueue<String> queue = new WaitableQueue<>();
		Thread[] enqueuers = new Thread[10000];
		Thread[] dequeuers = new Thread[10000];
		int i = 0;
		Random rand = new Random();
		
		for (i = 0; i < 10000; ++i) {
			enqueuers[i] = new Thread(new Runnable() { 
				@Override
				public void run() 
				{ 
					queue.enqueue("abc" + rand.nextInt(1000));
				} 
			}); 			
		}
	
		for (i = 0; i < 10000; ++i) {
			dequeuers[i] = new Thread(new Runnable() { 
				@Override
				public void run() 
				{ 
					queue.dequeue();
				} 
			}); 			
		}

		for (i = 0; i < 10000; ++i) {
			enqueuers[i].start();
			dequeuers[i].start();
		}
	}

	@Test
	void WhileThreadTest() throws InterruptedException {
		WaitableQueue<String> queue = new WaitableQueue<>();
		Random rand = new Random();
		
		Thread enqueuer = new Thread(new Runnable() { 
			@Override
			public void run() 
			{ 
				while (true) { queue.dequeue(); } 
			}
		}); 
		
		Thread dequeuer =  new Thread(new Runnable() { 
			@Override
			public void run() 
			{ 
				while (true) { queue.enqueue("abc" + rand.nextInt(1000)); }
			} 
		}); 	
		
		enqueuer.start();
		dequeuer.start();
	//	enqueuer.join();
	//	dequeuer.join();
	}
	
	@Test
	void strTest() {
		Dog dog1 = new Dog("Rex", 7, "Brown");
		
		String str1 = "Hello";
		String str2 = "lloHe";
		
		assertTrue(dog1.isPermutation(str1, str2));
	}
	*/	
}


