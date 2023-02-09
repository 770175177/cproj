#include<stdio.h>
#include<sys/stat.h>
#include<stdlib.h>
#include<string.h>
#include<fcntl.h>
#include<sys/types.h>
#include<unistd.h>
#include<sys/wait.h>

int main()
{
	pid_t pid;
	int i, fd;
	char *buf = "This is a Daemon\n";

	pid = fork();  //step one
	if(pid < 0)
	{
		printf("Error fork\n");
		exit(1);
	}
	else if(pid > 0)
	{
		exit(0);
	}

	setsid();	//step two
	chdir("/tmp"); 	//step three

	umask(0);	//step four
	for(i = 0; i < getdtablesize(); i++)	//step five
	{
		close(i);
	}

	while(1)	//do it
	{
		if((fd = open("./daemon.log",O_CREAT|O_WRONLY|O_TRUNC,0600)) < 0)
		{
			printf("Open file error\n");
			exit(1);
		}
		write(fd,buf,strlen(buf));
		close(fd);
		sleep(2);
	}

	exit(0);
}
