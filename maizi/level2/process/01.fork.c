#include<stdio.h>
#include<unistd.h>
#include<sys/types.h>
#include<stdlib.h>

int main()
{
	pid_t pid;
	pid = fork();

	if(pid == 0)
	{
		int i;
		for(i=0; i<5; i++)
		{
			printf("this is child process i=%d\n", i);
			usleep(100);
		}
	}
	if(pid > 0)
	{
		int i;
		for(i=0; i<5; i++)
		{	
			printf("this is parent process i=%d\n", i);
			usleep(100);
		}
	}
	while(1);

	return 0;
}
