#define _POSIX_C_SOURCE 199309L
#include <stdio.h>
#include <sys/types.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <signal.h>

static volatile int signalPid = -1;

void get_pid(int sig, siginfo_t *info, void *context)
{
    signalPid = info->si_pid;
	puts("PING");
}

int main()
{
	struct sigaction sa;

	sa.sa_flags = SA_SIGINFO;
    sa.sa_sigaction = get_pid;
    sigaction(SIGUSR2, &sa, NULL);
       
	while(1)
	{
		pause();
		sleep(1);
		kill(signalPid, SIGUSR1);
	}

	return 0;
}
