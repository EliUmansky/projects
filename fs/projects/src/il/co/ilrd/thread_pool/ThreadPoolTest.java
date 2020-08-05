package il.co.ilrd.thread_pool;

import static org.junit.jupiter.api.Assertions.*;

import java.util.concurrent.Callable;
import java.util.concurrent.ExecutionException;
import java.util.concurrent.Future;
import java.util.concurrent.TimeUnit;
import java.util.concurrent.TimeoutException;

import org.junit.jupiter.api.Test;

import il.co.ilrd.thread_pool.ThreadPool.Priority;
import il.co.ilrd.waitable_queue.WaitableQueue;


public class ThreadPoolTest {

	@Test
	void test1() throws InterruptedException, ExecutionException, TimeoutException {
		ThreadPool tp = new ThreadPool(2); 
		Future<?> f= null;
		for (int i = 0; i < 10; ++i) {
			f = tp.submit(new Runnable() {
				@Override
				public void run() {
					System.out.println("first task done!!!!");
					
				}
				
			} , Priority.MID);			
		}

		assertNull(f.get());
	}

	@Test
	void test2() throws InterruptedException, ExecutionException, TimeoutException {
		ThreadPool tp = new ThreadPool(100); 
		Future<String> f = null;
		for (int i = 0; i < 1000; ++i) {
			f = tp.submit(new Callable<>() {
				@Override
				public String call() {
					return "second task done!!!!";
				}
				
			} , Priority.MID);		
		}

		assertEquals("second task done!!!!", f.get());
		
		tp.shutdown();
	/*	Submitting after shutdown throws RejectedExecutionException 
		f = tp.submit(new Callable<>() {
			@Override
			public String call() {
				return "second task done!!!!";
			}
			
		} , Priority.MID);	*/
		
		assertTrue(tp.awaitTermination(5, TimeUnit.SECONDS));
	
	}

	@Test
	void test3() throws InterruptedException, ExecutionException, TimeoutException {
		ThreadPool tp = new ThreadPool(4); 
		WaitableQueue<Integer> q = new WaitableQueue<>();
		
		for (int i = 10; i < 30; ++i) {
			q.enqueue(i);
		}
		
		Future<String> f = null;
		for (int i = 10; i < 20; ++i) {
			f = tp.submit(new Callable<>() {
				@Override
				public String call() {
					System.out.print(q.dequeue() + " ");
					return "third task done!!!!";
				}
				
			} , Priority.MID);	
		}
		assertEquals("third task done!!!!", f.get(2, TimeUnit.SECONDS));
		
		tp.pause();
		
		for (int i = 20; i < 30; ++i) {
			f = tp.submit(new Callable<>() {
				@Override
				public String call() {
					System.out.print(q.dequeue() + " ");
					return "third task done!!!!";
				}
				
			} , Priority.MID);	
		}
		System.out.println("\nwaiting for resume");
		tp.resume();
		tp.shutdown();
		assertFalse(tp.awaitTermination(1, TimeUnit.NANOSECONDS));
	}

	@Test
	void test4() throws InterruptedException, ExecutionException, TimeoutException {
		ThreadPool tp = new ThreadPool(4); 
		
		Callable<String> callableLow = new Callable<>() {
			@Override
			public String call() {
				System.out.print("\nlow priority");
				return "fourth task done!!!!";
			}		
		};
		
		Callable<String> callableHigh = new Callable<>() {
			@Override
			public String call() {
				System.out.print("\nhigh priority");
				return "fourth task done!!!!";
			}		
		};

		
		Future<String> f = null;
		for (int i = 0; i < 20; ++i) {
			f = tp.submit(callableHigh, Priority.HIGH);
			f = tp.submit(callableLow, Priority.LOW);
		}
		String str = f.get();
		assertEquals(str, "fourth task done!!!!");
	}

	@Test
    void pauseSetResume() {
        ThreadPool pool = new ThreadPool(10);
        pool.pause();
        pool.setNumOfThreads(5);
        pool.resume();
        Callable<String> callable = ()->{
            assertEquals(1, 1);
            return "end";
        };
        Future<String> f = pool.submit(callable);
        pool.shutdown();
        try {
            System.out.println(pool.awaitTermination(10, TimeUnit.SECONDS));
        } catch (InterruptedException e) {
            e.printStackTrace();
        }
        try {
			assertEquals("end", f.get());
		} catch (InterruptedException | ExecutionException e) {
			e.printStackTrace();
		}
        System.out.println("finish");

    }

	@Test
    void cancel() {
        ThreadPool pool = new ThreadPool(10);
       
        Callable<String> callable = ()->{
            return "end";
        };
        Future<String> f = pool.submit(callable);
        f.cancel(true);
        pool.shutdown();
    /*    f.get() will throw CancellationException
        try {
			System.out.println(f.get());
		} catch (InterruptedException | ExecutionException e) {
			e.printStackTrace();
		} */
    }
	
	@Test
    void get() throws InterruptedException, ExecutionException {
		
        ThreadPool pool = new ThreadPool(10);
        Future<String> f = null;
        Future<String> future = null;
        Callable<String> callable = ()->{
            return "end";
        };
        
        Callable<String> callableComplex = ()->{
            double num = 0;
            
            while (true) {
            	if (200000000 == num) {
            		break;
            	}
            	++num;
            }
        	return "not end";
        };
        
        for (int i = 0; i < 100; ++i) {
        	f = pool.submit(callable);   
        	try {
				assertEquals("end", f.get());
			} catch (InterruptedException | ExecutionException e) {
				e.printStackTrace();
			}
        }
        
        f = pool.submit(callable);  
        try {
			System.out.println(f.get());
		} catch (InterruptedException | ExecutionException e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
		}
       
        try {
			System.out.println(f.get());
		} catch (InterruptedException | ExecutionException e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
		}
        
        f = pool.submit(callableComplex);
        System.out.println(f.get());
        f = pool.submit(callableComplex);
        System.out.println(f.get());
/*
        try {
        	assertEquals("end", f.get(1, TimeUnit.SECONDS));
        } catch (InterruptedException | ExecutionException | TimeoutException e) {
        	e.printStackTrace();
        }*/
        future = pool.submit(callableComplex);  
    /* Throws TimeoutException     
        try {
			assertEquals("end", future.get(1, TimeUnit.NANOSECONDS));
		} catch (InterruptedException | ExecutionException | TimeoutException e) {
			e.printStackTrace();
		} 
		*/
        pool.shutdown();
 
    }
}
