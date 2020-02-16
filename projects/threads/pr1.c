#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <semaphore.h>
#include <fcntl.h>
#include <sys/stat.h>

int main()
{
	sem_t *sem1 = sem_open("/Sem1", O_APPEND);
	sem_t *sem2 = sem_open("/Sem2", O_APPEND);

	while (1)
	{
		sleep(1);
		sem_wait(sem1);
		fprintf(stderr, "1 ");
		sem_post(sem2);
	}
		
	return 0;
}
