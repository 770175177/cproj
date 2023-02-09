#include<stdio.h>
#include<sys/types.h>
#include<stdlib.h>
#include<signal.h>
#include<fcntl.h>

int main(int argc, char *argv[])
{
	pid_t pid;
	
	pid = fork();	
	if(pid > 0)
	{
		sleep(8);
		if(waitpid(pid, NULL, WNOHANG) == 0)
		{
			kill(pid, 9);
		}
	}
	if(pid == 0)
	{
		printf("raise function before\n");
		raise(SIGTSTP);
		printf("raise function after\n");
		exit(0);
	}

	return 0;
}
