
#define _POSIX_SOURCE
#include<unistd.h> 
#include <stdio.h>
#include <sys/types.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <signal.h>

void ChildHandler(int signal_num)
{
	signal(SIGUSR1, &ChildHandler);
	(void)signal_num;
	puts("PING");
}

int main() 
{ 
	int parent_id = getppid();

	signal(SIGUSR1, &ChildHandler);	
	
	while(1)
	{
		pause();
		sleep(1);
		kill(parent_id, SIGUSR1);
	}

    return 0; 
} 

