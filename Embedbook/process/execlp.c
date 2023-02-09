#include<stdio.h>
#include<unistd.h>
#include<stdlib.h>
#include<sys/types.h>

int main()
{
	pid_t ret;
	if(fork() == 0)
	{
		if((ret = execlp("ps","ps","-ef",NULL)) < 0)
		{
			printf("execlp error\n");
		}
	}
}
