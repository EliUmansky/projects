package il.co.ilrd.waitable_queue;

import java.util.Comparator;
import java.lang.Comparable;
import java.util.PriorityQueue;
import java.util.Queue;
import java.util.concurrent.Semaphore;
import java.util.concurrent.TimeUnit;
import java.util.concurrent.locks.Lock;
import java.util.concurrent.locks.ReentrantLock;

public class WaitableQueue<E> {
	
	private Queue<E> queue;
	Semaphore sem = new Semaphore(0);
	Lock lock = new ReentrantLock();

	public <T extends Comparable<T>> WaitableQueue() {
		queue = new PriorityQueue<>();
	}

	public WaitableQueue(Comparator<E> comparator) {
		queue = new PriorityQueue<>(comparator);
	}

	public void enqueue(E element) {
		if (null == element) { throw new NullPointerException(); }
		
		lock.lock();
		queue.add(element);
		lock.unlock();
		sem.release();
	}
	
	public E dequeue() {
		try {
			sem.acquire();
		} catch (InterruptedException e) {
			e.printStackTrace();
		}
		
		lock.lock();
	
		E element = queue.poll();
		lock.unlock();

		return element;
	}
	
	public E dequeueWithTimeout(long timeout, TimeUnit unit) throws InterruptedException {
		long endTime = System.currentTimeMillis() + TimeUnit.MILLISECONDS.convert(timeout, unit);
		E element = null;
		
		while (System.currentTimeMillis() < endTime && null == element) {
			if (!queue.isEmpty()) {
				lock.lock();
			
				if (!queue.isEmpty()) {
					sem.tryAcquire(endTime - System.currentTimeMillis(), TimeUnit.MILLISECONDS);
					element = queue.poll();
				}
				lock.unlock();						
			}	
		}
		
		return element;
	}
}
