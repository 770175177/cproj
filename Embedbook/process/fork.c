#include<stdio.h>
#include<sys/types.h>
#include<unistd.h>

int main()
{
	pid_t ret;

	ret = fork();

	if(ret == -1)
	{
		perror("fork error");
		return -1;
	}
	else if(ret == 0)
	{
		printf("In child process!!ret is %d,My PID is %d\n",ret,getpid());
	}
	else
	{
		printf("In parent process!!ret is %d,My PID is %d\n",ret,getpid());

	return 0;
	}
}
