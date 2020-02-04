#define _POSIX_C_SOURCE 199309L
#include <stdio.h>
#include <sys/types.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <signal.h>


void ChildHandler(int signal_num)
{
	signal(SIGUSR1, &ChildHandler);
	(void)signal_num;
	puts("PONG");
}

int main(int argc, char **argv)
{
	int arg = (int)strtol(argv[1], NULL, 10);	

	signal(SIGUSR1, &ChildHandler);
	
	while(1)
	{
		sleep(1);
		kill(arg, SIGUSR2);
		pause();
	}

	return 0;
}
