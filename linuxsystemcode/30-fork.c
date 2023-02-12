#include <stdio.h>
#include <unistd.h>
#include <errno.h>
#include <time.h>

int main()
{
	pid_t pid;

	if((pid = fork()) < 0)
	{
		perror("fork");
		return -1;
	}
	if(pid == 0)
	{
		printf("this is son, pid is %d, ppid is %d\n", getpid(), getppid());
	}
	else
	{
		printf("this is father, pid is %d, ppid is %d\n", getpid(), getppid());
		sleep(1);
	}

	return 0;
}
