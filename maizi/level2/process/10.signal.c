#include<stdio.h>
#include<unistd.h>
#include<stdlib.h>
#include<signal.h>
#include<fcntl.h>

int main(int argc, char *argv[])
{
	int sig;
	int pid;

	if(argc < 3)
	{
		printf("please input param\n");
		return -1;
	}
	sig = atoi(argv[1]);
	pid = atoi(argv[2]);

	printf("sig=%d, pid=%d\n", sig, pid);
	kill(pid, sig);
	return 0;
}
