
#define _POSIX_SOURCE
#include<unistd.h> 
#include <stdio.h>
#include <sys/types.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <signal.h>
void ParentHandler(int signal_num)
{
	signal(SIGUSR2, &ParentHandler);
	(void)signal_num;
	puts("PING");
}

int main() 
{ 
	signal(SIGUSR2, &ParentHandler);

    return 0; 
} 
