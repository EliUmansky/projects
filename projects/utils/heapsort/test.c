#include <stdio.h>

int main()
{
	int arr[10] = {8, 5, 2, 9, 6, 3, 7, 4, 1, 10};
	int i = 0;

	HeapSort(arr, 10);

	for (; i < 10; ++i)
	{
		printf("%d ", arr[i]);
	}

	return 0;
}	
