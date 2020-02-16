#include <stdio.h>
#include <string.h>
#include <pthread.h>
#include <stdlib.h>
#include <unistd.h>
#include <semaphore.h>

#define NUM_OF_CONSUMERS (5)

size_t data = 0;
int version = 0;
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
sem_t consumer_num;
pthread_cond_t cv_flag;

void *Producer(void *arg)
{
	int i = 0;

	while (1)
	{
		for	(i = 0; i < NUM_OF_CONSUMERS; ++i)
		{	
			sem_wait(&consumer_num);
		}		
		pthread_mutex_lock(&mutex);	
		++data;
		++version;
		fprintf(stderr, "\nProducer: %ld \n", data);

		pthread_cond_broadcast(&cv_flag);
		pthread_mutex_unlock(&mutex);	
	}
	
	(void)arg; 
	return NULL;
}

void *Consumer(void *arg)
{
	int current = version;
	while (1)
	{	
		pthread_mutex_lock(&mutex);				
		sem_post(&consumer_num);
		while (current == version)
		{		
			pthread_cond_wait(&cv_flag, &mutex);
		}
		current = version;
		fprintf(stderr, "Consumer: %ld \n", data);
		pthread_mutex_unlock(&mutex);
	} 


	(void)arg;
	return NULL;
}

int main()
{
	int i = 0;
	pthread_t tid[6] = {0};	
	
	sem_init(&consumer_num, 0, 0);
	pthread_create(&tid[0], NULL, &Producer, NULL);
	
	for (i = 0; i < NUM_OF_CONSUMERS; ++i)
	{
		pthread_create(&tid[i + 1], NULL, &Consumer, NULL);
	}

	for (i = 0; i <= NUM_OF_CONSUMERS; ++i)
	{
		pthread_join(tid[i], NULL);  
	}

	return 0;
}
