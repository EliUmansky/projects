#include <stdio.h>
#include <string.h>
#include <pthread.h>
#include <stdlib.h>
#include <unistd.h>
#include <omp.h>
#include <stdio.h>
#include <math.h>

#define SIZE (10000000)
#define NUM_OF_THREADS (1)

void *Func(void *num_of_threads)
{
	size_t i = 10000;
	size_t j = 0;

	#pragma omp parallel for
	for (j = 0; j < SIZE/NUM_OF_THREADS; ++j)
	{
		i = (size_t)sqrt(i) * 45 * (size_t)pow(i, 13);
		i %= 13 * 34;
		i = (size_t)pow(i, 13) * 56 * (size_t)pow(i, 13);
		i /= 666 * 12;
		i = (size_t)sqrt(i) * 456 * (size_t)pow(i, 13);
		i = (size_t)sqrt(i) * 45 * (size_t)pow(i, 13);
		i %= 13 * 34;
		i = (size_t)pow(i, 13) * 56 * (size_t)pow(i, 13);
		i /= 666 * 12;
		i = (size_t)sqrt(i) * 456 * (size_t)pow(i, 13);
		i = (size_t)sqrt(i) * 45 * (size_t)pow(i, 13);
		i %= 13 * 34;
		i = (size_t)pow(i, 13) * 56 * (size_t)pow(i, 13);
		i /= 666 * 12;
		i = (size_t)sqrt(i) * 456 * (size_t)pow(i, 13);
	}
	(void)num_of_threads;

	return NULL;
}

void Detach()
{
	size_t i = 0;
	pthread_t ptid[NUM_OF_THREADS] = {0};

    for (i = 0; i < NUM_OF_THREADS; ++i)
    {
		pthread_create(&ptid[i], NULL, &Func, (void*)NUM_OF_THREADS);
	}

	for (i = 0; i < NUM_OF_THREADS; ++i)
    {
		pthread_join(ptid[i], NULL);  
	}

}

int main(void)
{
	Detach();

    return 0;
}
