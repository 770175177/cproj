#include<stdio.h>
#include<sys/types.h>
#include<stdlib.h>
#include<signal.h>

void myfun(int signum)
{
	int i = 0;
	while(i < 10)
	{
		printf("process signal signum=%d\n", signum);
		sleep(1);
		i++;
	}
}

int main(int argc, char *argv[])
{
	int i = 0;
	
	signal(14, myfun);
	printf("alarm before\n");
	alarm(9);
	printf("alarm after\n");
//	signal(14, SIG_IGN);
	signal(14, SIG_DFL);
	while(i < 20)
	{
		i++;
		sleep(1);
		printf("process things, i=%d\n", i);
	}

	return 0;
}
