#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <semaphore.h>
#include <fcntl.h>
#include <sys/stat.h>

int main()
{
	sem_t *sem3 = sem_open("/Sem3", O_APPEND);
	sem_t *sem1 = sem_open("/Sem1", O_APPEND);

	while (1)
	{
		sem_wait(sem3);
		fprintf(stderr, "3\n");
		sem_post(sem1);
	}
		
	return 0;
}
