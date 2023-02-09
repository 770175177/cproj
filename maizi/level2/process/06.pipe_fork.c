#include<stdio.h>
#include<unistd.h>
#include<sys/types.h>
#include<stdlib.h>

int main()
{
	pid_t pid;
	int fd[2];
	int ret;
	char process_inter=0; 

	ret = pipe(fd);
	if(ret < 0)
	{
		printf("create pipe failure\n");
		return -1;
	}
	printf("create pipe success\n");

	pid = fork();	//child process
	if(pid == 0)
	{
		int i;
		read(fd[0], &process_inter, 1);
		while(process_inter== 0);
		for(i=0; i<5; i++)
		{
			printf("this is child process i=%d\n", i);
			usleep(100);
		}
	}
	if(pid > 0) //parent process
	{
		int i;
		for(i=0; i<5; i++)
		{	
			printf("this is parent process i=%d\n", i);
			usleep(100);
		}
		process_inter = 1;
		write(fd[1], &process_inter, 1);
	}
	while(1);

	return 0;
}
