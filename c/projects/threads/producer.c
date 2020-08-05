#include <stdio.h>
#include <string.h>
#include <pthread.h>
#include <stdlib.h>
#include <unistd.h>

#define FREE (0)
#define NOT_FREE (1)

volatile int flag = FREE;

void *Producer(void *count)
{
	while (1)
	{
		if (FREE == flag)
		{
			++*(int*)count;
			fprintf(stderr, "%d ", *(int*)count);
			flag = NOT_FREE;
		}
	}
	
	return NULL;
}

void *Consumer(void *count)
{
	while (1)
	{
		if (NOT_FREE == flag)
		{
			fprintf(stderr, "%d \n", *(int*)count);
			flag = FREE;
		}
	} 


	return NULL;
}

int main()
{
	pthread_t tid[2] = {0};	
	int count = 0;

	pthread_create(&tid[0], NULL, &Producer, &count);
	pthread_create(&tid[1], NULL, &Consumer, &count);
	pthread_join(tid[0], NULL);  
	pthread_join(tid[1], NULL);  

	return 0;
}
