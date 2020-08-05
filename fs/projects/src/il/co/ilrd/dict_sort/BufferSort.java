package il.co.ilrd.dict_sort;

import java.io.File;
import java.io.FileWriter;
import java.io.IOException;
import java.nio.file.Files;
import java.nio.file.Paths;
import java.util.ArrayList;
import java.util.Collections;
import java.util.List;
import java.util.Scanner;

public class BufferSort 
{ 
	int histogram[] = new int[256]; 

	public void Sort() {
		char arr[] = CreateArray(); 
		final int NUM_OF_THREADS = 4;
		
		Thread threadArr[] = new Thread[NUM_OF_THREADS];
		long startTime = System.currentTimeMillis();
		for (int i = 0; i < NUM_OF_THREADS; ++i) {
			threadArr[i] = new Thread(new Threads(histogram, arr, i * arr.length / NUM_OF_THREADS, ((i + 1) * arr.length) / NUM_OF_THREADS));
			threadArr[i].start();
			try {
				threadArr[i].join();
			} catch (InterruptedException e) {
				e.printStackTrace();
			} 
		}
		arr = Merge(arr);
		long endTime = System.currentTimeMillis();
		long execTime = endTime - startTime;
		System.out.println("Total execution time: " + execTime);
		
		System.out.print("Occurrence of character " + (char)0 + "- ");
		System.out.println(histogram[0]);
		for (int i = 1; i < 256; ++i) {
			System.out.print("Occurrence of character " + (char)i + "- ");
			System.out.println(histogram[i] - histogram[i - 1]);
		}
	}
	
	public static void Sort(int[] histogram, char[] arr, int startIndex, int endIndex, int[] localHistogram) { 
		for (int i = startIndex; i < endIndex; ++i) {++localHistogram[arr[i]];}
		
		synchronized(histogram) {
			for (int i = 0; i < histogram.length; ++i) {
				histogram[i] += localHistogram[i];
			}
		}
	} 

	public char[] CreateArray() {
		String str = null;
		String filePath= System.getProperty("user.dir") + "/src/il/co/ilrd/dict_sort/dictionary.txt";
	
		filePath = filePath.replace('\\', '/');
		try {
			str = new String (Files.readAllBytes(Paths.get(filePath)));
		} catch (IOException e) {
			e.printStackTrace();
		}
		
		return str.toCharArray();
	}
	
	public char[] Merge(char[] arr) {
		char[] output = new char[arr.length];

		for (int i = 1; i <= 255; ++i) 
			histogram[i] += histogram[i - 1]; 

		for (int i = arr.length - 1; i >= 0; --i) 
		{ 
			output[histogram[arr[i]] - 1] = arr[i]; 
			--histogram[arr[i]]; 
		} 

		return output;  
	}
	
	public void Shuffle(String filePath) throws IOException {
		File file = new File(filePath);
		Scanner text = new Scanner(file);
		List<String> lines = new ArrayList<String>();
		while (text.hasNextLine()) {
			lines.add(text.nextLine());
		}
		text.close();
		Collections.shuffle(lines);
		
		FileWriter writer = new FileWriter("output.txt");
		for (String line : lines) {
			writer.write(line + System.lineSeparator());
		}
		writer.close();
	}	
	
	public static void main(String args[]) 
	{ 
		BufferSort obj = new BufferSort();
		try {
			obj.Shuffle(System.getProperty("user.dir") + "/src/il/co/ilrd/dict_sort/dict.txt");
		} catch (IOException e) {
			e.printStackTrace();
		}
		obj.Sort();
	}
} 