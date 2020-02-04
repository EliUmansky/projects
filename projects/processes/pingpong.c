#define _POSIX_SOURCE
#include <stdio.h>
#include <sys/types.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <signal.h>

void OnlyFork();
void ForkAndExec();
/*void ParentHandler(int signal_num);
void ChildHandler(int signal_num);
*/
int main()
{	
/*	OnlyFork();
*/	ForkAndExec();

	return 0;
}

void OnlyFork()
{
	int parent_pid = 0;
	int pid = fork();

	signal(SIGUSR1, &ChildHandler);
	signal(SIGUSR2, &ParentHandler);

	while(1)
	{
		if(pid)
		{
			pause();
			sleep(1);			
			kill(pid, SIGUSR1);
		}
		else
		{
			parent_pid = getppid();
			kill(parent_pid, SIGUSR2);
			pause();
		}
	}
}

void ForkAndExec()
{
	char *args[]={"./child", NULL};
	int pid = fork();

	signal(SIGUSR1, &ChildHandler);

	while(1)
	{
		if(pid)
		{
			sleep(1);
			kill(pid, SIGUSR1);
			pause();
		}
		else
		{
			execvp(args[0], args);
		}
	}
}

void ParentHandler(int signal_num)
{
	signal(SIGUSR2, &ParentHandler);
	(void)signal_num;
	puts("PING");
}

void ChildHandler(int signal_num)
{
	signal(SIGUSR1, &ChildHandler);
	(void)signal_num;
	puts("PONG");
}
