#include<stdio.h>
#include<sys/types.h>
#include<stdlib.h>
#include<signal.h>

void myfun1(int signum)
{
	int i = 0;
	while(i < 5)
	{
		printf("recv signum=%d, i=%d\n", signum, i);
		sleep(1);
		i++;
	}
	return ;
}

void myfun2(int signum)
{
	printf("recv signum=%d\n", signum);
	wait(NULL);
	return ;
}

int main(int argc, char *argv[])
{
	pid_t pid;
	pid = fork();
	if(pid > 0)
	{
		int i = 0;
		signal(10, myfun1);
		signal(17, myfun2);
		while(1)
		{
			printf("parent process things, i=%d\n", i);
			sleep(1);
			i++;
		}
	}
	if(pid == 0)
	{
		sleep(10);
		kill(getppid(), 10);
		sleep(10);
		exit(0);
	}

	return 0;
}
