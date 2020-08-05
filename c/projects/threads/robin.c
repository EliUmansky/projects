#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <semaphore.h>
#include <fcntl.h>
#include <sys/stat.h>

#define NUM_OF_CHILDREN (2)


int main()
{
	pid_t pid;
	char *args[] = {"./pr1", "./pr2", "./pr3", NULL};
	int i = 0;

	sem_open("/Sem1", O_CREAT, 0644, 1);
	sem_open("/Sem2", O_CREAT, 0644, 0);
	sem_open("/Sem3", O_CREAT, 0644, 0);

	for (; i < NUM_OF_CHILDREN; ++i) 
	{
    	pid = fork();
    	if (pid == 0) 
		{
			execvp(args[i + 1], args);
	    }
	}
 
	execvp(args[0], args);

	return 0;
}

