#include <stdio.h>
#include <sys/types.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

int main()
{
	char *args[]={"./exe", NULL};
	int pid = 0;

	while(1)
	{
		pid = fork();
		if(pid > 0)
		{
			printf("parent pid = %d\n", getpid());
			wait(NULL);
		}
		else
		{
			printf("child pid = %d\n", getpid());
			execvp(*args, args);
			return 0;
		}
	}

	return 0;
}

