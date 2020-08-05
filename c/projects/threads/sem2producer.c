#include <stdio.h>
#include <string.h>
#include <pthread.h>
#include <stdlib.h>
#include <unistd.h>
#include <semaphore.h>

#include "queue.h"

#define MAX_SIZE (5)

size_t data = 1;
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
sem_t q_size;
sem_t space_left;
queue_t *queue;

void *Producer(void *arg)
{
	while (1)
	{
		sem_wait(&space_left);
		pthread_mutex_lock(&mutex);	
		QueEnqueue(queue, (void *)data);		
		fprintf(stderr, "Enqueue %ld ", (size_t)data);
		++data;
		sem_post(&q_size);
		pthread_mutex_unlock(&mutex);	
	}
	
	(void)arg; 
	return NULL;
}

void *Consumer(void *arg)
{
	while (1)
	{		
		sem_wait(&q_size);
		pthread_mutex_lock(&mutex);
		fprintf(stderr, "Dequeue %ld \n", (size_t)QuePeek(queue));
		QueDequeue(queue);
		sem_post(&space_left);
		pthread_mutex_unlock(&mutex);
	} 


	(void)arg;
	return NULL;
}

int main()
{
	pthread_t tid[8] = {0};	
	queue = QueCreate();
	sem_init(&q_size, 0, 0);
	sem_init(&space_left, 0, MAX_SIZE);

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
