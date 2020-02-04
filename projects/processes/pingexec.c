#include <stdio.h>
#include <sys/types.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

int main()
{
	char *args[]={"./parent", "./child", NULL};
	int pid = fork();

	while(1)
	{
		if(pid > 0)
		{
			execvp(args[0], args);
			wait(NULL);
		}
		else
		{
			execvp(args[1], args);
		}
	}

	return 0;
}

