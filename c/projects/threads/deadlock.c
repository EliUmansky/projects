#include <stdio.h>
#include <string.h>
#include <pthread.h>
#include <stdlib.h>
#include <unistd.h>

#include "queue.h"

size_t data = 1;
pthread_mutex_t mutex1 = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t mutex2 = PTHREAD_MUTEX_INITIALIZER;

void *Producer(void *arg)
{
	while (1)
	{
		pthread_mutex_lock(&mutex1);	
		pthread_mutex_lock(&mutex2);	
		fprintf(stderr, "Producer: %ld ", (size_t)data);
		++data;
		pthread_mutex_unlock(&mutex1);	
		pthread_mutex_unlock(&mutex2);		
	}
	
	(void)arg;
	return NULL;
}

void *Consumer(void *arg)
{
	while (1)
	{	
		pthread_mutex_lock(&mutex2);	
		pthread_mutex_lock(&mutex1);	
		fprintf(stderr, "Consumer: %ld ", (size_t)data);
		pthread_mutex_unlock(&mutex2);	
		pthread_mutex_unlock(&mutex1);		
	} 

	(void)arg;
	return NULL;
}

int main()
{
	pthread_t tid[8] = {0};	

	pthread_create(&tid[0], NULL, &Producer, NULL);
	pthread_create(&tid[1], NULL, &Consumer, NULL);
	pthread_create(&tid[2], NULL, &Producer, NULL);
	pthread_create(&tid[3], NULL, &Consumer, NULL);
	pthread_create(&tid[4], NULL, &Producer, NULL);
	pthread_create(&tid[5], NULL, &Consumer, NULL);
	pthread_create(&tid[6], NULL, &Producer, NULL);
	pthread_create(&tid[7], NULL, &Consumer, NULL);

	pthread_join(tid[0], NULL);  
	pthread_join(tid[1], NULL);
	pthread_join(tid[2], NULL);  
	pthread_join(tid[3], NULL);
	pthread_join(tid[4], NULL);  
	pthread_join(tid[5], NULL);
	pthread_join(tid[6], NULL);  
	pthread_join(tid[7], NULL);  

	return 0;
}
