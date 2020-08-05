#include <stdio.h>
#include <string.h>
#include <pthread.h>
#include <stdlib.h>
#include <unistd.h>

#define SIZE (100000)

size_t arr[SIZE];
pthread_t tid[SIZE];

void *Func(void *i)
{
    arr[(size_t)i] = (size_t)i;
	
	return NULL;
}

void Single()
{
    size_t i = 0;
	int err = 0;

    while (i < SIZE)
    {
        err = pthread_create(&tid[i], NULL, &Func, (void*)i);
        if (err != 0)
        {
			printf("\ncan't create thread :[%s]", strerror(err));
			break;
		}      
		pthread_join(tid[i], NULL);  
		++i;
	}

	for (i = 0; i < SIZE; ++i)
	{
		printf("%ld ", arr[i]);
	}
}

void Detach()
{
	pthread_attr_t attr;
	size_t i = 0;
	int err = 0;

    while (i < SIZE)
    {
		pthread_attr_init(&attr);  
		pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_DETACHED);      
		err = pthread_create(&tid[i], &attr, &Func, (void*)i);
        if (err != 0)
        {
			printf("\ncan't create thread :[%s]", strerror(err));
			break;
		}      
		pthread_attr_destroy(&attr); 
		++i;
	}

	sleep(1);
	
	for (i = 0; i < SIZE; ++i)
	{
		printf("%ld ", arr[i]);
	}
}

int main(void)
{
	Single();
	Detach();

    return 0;
}
