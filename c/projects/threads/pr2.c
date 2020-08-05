#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <semaphore.h>
#include <fcntl.h>
#include <sys/stat.h>

int main()
{
	sem_t *sem2 = sem_open("/Sem2", O_APPEND);
	sem_t *sem3 = sem_open("/Sem3", O_APPEND);

	while (1)
	{
		sem_wait(sem2);
		fprintf(stderr, "2 ");
		sem_post(sem3);
	}
		
	return 0;
}
