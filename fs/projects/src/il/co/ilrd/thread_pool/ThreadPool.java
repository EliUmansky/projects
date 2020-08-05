package il.co.ilrd.thread_pool;

import java.util.Comparator;
import java.util.concurrent.Callable;
import java.util.concurrent.CancellationException;
import java.util.concurrent.CountDownLatch;
import java.util.concurrent.Executors;
import java.util.concurrent.Future;
import java.util.concurrent.RejectedExecutionException;
import java.util.concurrent.Semaphore;
import java.util.concurrent.TimeUnit;
import java.util.concurrent.TimeoutException;

import il.co.ilrd.waitable_queue.WaitableQueue;

public class ThreadPool {

	private int totalThreads;
	private int pausedThreads;
	private WaitableQueue<Task<?>> queue;
	private WaitableQueue<WorkerThread> endedThreads;
	private Semaphore waitSem = new Semaphore(0);
	private volatile boolean shutdownflag = false;
	
	public ThreadPool(int totalThreads) {
		queue = new WaitableQueue<>();
		this.totalThreads = totalThreads;
		for (int i = 0; i < totalThreads; ++i) {
			new WorkerThread().start();
		}
	}
	
	private class WorkerThread extends Thread {
		
		private volatile boolean isThreadDisabled = false;
		@Override
		public void run() {
			while(!isThreadDisabled) {
				try {
					queue.dequeue().executeTask();
				} catch (Exception e) {
					System.out.println("Thread ID: " + Thread.currentThread().getId());
					System.out.println("Cause of exception" + e.getCause());
				}
			}
		}
		
		private void disable() {
			isThreadDisabled = true;
		}
	}
	
	public enum Priority {
	
		LOW (1),
		MID (2),
		HIGH (3);
		
		private int priority;
		
		private Priority(int priority) {
			this.priority = priority;
		}
		
		public int getPriority() {
			return priority;
		}
	}
	
	private class Task<T> implements Comparable<Task<T>>{
		private int priority;
		private Callable<T> callable;
		private FutureImp<T> future;
	
		private Task(Callable<T> callable, int priority){
			this.priority = priority;
			this.callable = callable;
			future = new FutureImp<>();
		}
		
		private void executeTask() throws Exception {
			if(!future.isCancelled) {
				try {
					future.result = callable.call();
				} finally {
					future.isDone = true;
					future.waitForResult.countDown();
				}
			}
		}
		
		@Override
		public int compareTo(Task<T> task2) {
			return task2.priority - priority;
		}
		
		private class FutureImp<V> implements Future<V>{
			private volatile boolean isDone = false;
			private volatile boolean isCancelled = false;
			private V result = null;
			private CountDownLatch waitForResult = new CountDownLatch(1);
			
			@Override
			public boolean cancel(boolean arg0) {
				isCancelled = true;
				return !future.isDone();
			}

			@Override
			public V get() throws InterruptedException {
				if(isCancelled) { throw new CancellationException("The task was cancelled"); }	
				waitForResult.await();
				
				return result;	
			}

			@Override
			public V get(long timeout, TimeUnit unit) throws InterruptedException, TimeoutException {
				if(isCancelled) { throw new CancellationException("The task was cancelled"); }
				
				if (waitForResult.await(timeout, unit)) {
					return result;
				}
				
				throw new TimeoutException();									
			}
			
			@Override
			public boolean isCancelled() {return isCancelled;}
			@Override
			public boolean isDone() {return isDone;}
			
		}
	
	}

	public Future<?> submit(Runnable runnable, Priority priority) {
		return submit(Executors.callable(runnable), priority);
	}
	
	public Future<?> submit(Runnable runnable) {
		return submit(Executors.callable(runnable), Priority.MID);
	}
	
	public <T> Future<T> submit(Runnable runnable, Priority priority, T value) {
		return submit(Executors.callable(runnable, value), priority);
	}
	
	public <T> Future<T> submit(Callable<T> callable) {
		return submit(callable, Priority.MID);
	}
	
	public <T> Future<T> submit(Callable<T> callable, Priority priority) {
		if(shutdownflag){
			throw new RejectedExecutionException("Submit after shutdown");
		}
		
		try {
			Task<T> task = new Task<>(callable, priority.getPriority());
			queue.enqueue(task);
			
			return task.future;
		} catch(NullPointerException e) {
			throw new NullPointerException();			
		} catch(ClassCastException e) {
			throw new ClassCastException();			
		} 	
	}
	
	public void setNumOfThreads(int newNumOfThreads) {
		int oldNumOfThreads = totalThreads;
		totalThreads = newNumOfThreads;
				 
		if (newNumOfThreads < oldNumOfThreads) {
			for (int i = 0; i < oldNumOfThreads - newNumOfThreads; ++i) {
				queue.enqueue(new Task<>(Executors.callable(() -> ((WorkerThread)Thread.currentThread()).disable()), 
							  Priority.HIGH.getPriority() + 1));				
			}			
		} else {
			for (int i = 0; i < newNumOfThreads - oldNumOfThreads; ++i) {	
				new WorkerThread().start();
			}
		}
		
	}
	
	public void pause() {	
		pausedThreads = totalThreads;
		
		for (int i = 0; i < pausedThreads; ++i) {
			queue.enqueue(new Task<>(Executors.callable(() -> {
				try {
					waitSem.acquire();
				} catch (InterruptedException e) {
					e.printStackTrace();
				}
			}), Priority.HIGH.getPriority() + 1)); 
		}
	}
	
	public void resume() {
		waitSem.release(pausedThreads);
		pausedThreads = 0;
	}
	
	public void shutdown() {
		shutdownflag = true;
		Comparator<WorkerThread> comp = (WorkerThread t1, WorkerThread t2) -> {return 0;};
		endedThreads = new WaitableQueue<>(comp);
		
		for (int i = 0; i < totalThreads; ++i) {
			queue.enqueue(new Task<>(Executors.callable(() -> {
				WorkerThread wt = (WorkerThread)Thread.currentThread();
				endedThreads.enqueue(wt);
				wt.disable();
				}) , Priority.LOW.getPriority() - 1));				
		}
	}
	
	public boolean awaitTermination(long timeout, TimeUnit unit) throws InterruptedException {
		long endTime = System.currentTimeMillis() + TimeUnit.MILLISECONDS.convert(timeout, unit);
		
		for (int i = 0; i < totalThreads; ++i) {
			try {
				endedThreads.dequeueWithTimeout(endTime - System.currentTimeMillis(), TimeUnit.MILLISECONDS).join();
			} catch (NullPointerException e) {
				return false;
			} catch (InterruptedException e) {
				e.printStackTrace();
			}
		}
		
		return true;
	}
}
