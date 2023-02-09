#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>

int main()
{
	char *arg[] = {"env",NULL};
	char *envp[] = {"PATH=/tmp","USER=harry",NULL};

	if(fork() == 0)
	{
		if(execve("/usr/bin/env",arg,envp) < 0)
		{
			printf("execve error\n");
		}
	}
}
