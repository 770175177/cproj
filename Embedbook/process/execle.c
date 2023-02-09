#include<stdio.h>
#include<unistd.h>
#include<stdlib.h>

int main()
{
	char *envp[] = {"PATH=/tmp","USER=harry",NULL};

	if(fork() == 0)
	{
		if(execle("/usr/bin/env","env",NULL,envp) < 0)
		{
			printf("execle error\n");
		}
	}
}
