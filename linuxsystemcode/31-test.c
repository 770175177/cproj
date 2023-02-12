#include <stdio.h>
#include <unistd.h>
#include <errno.h>
#include <time.h>

int main()
{
	pid_t pid;
	char *arg[] = {"ls", "-a", NULL};

	if((pid = fork()) < 0)
	{
		perror("fork");
		return -1;
	}
	//son
	if(pid == 0)
	{
		printf("fork1 ok; execl");
		if(execl("./01-helloworld", "hello", "world", NULL) < 0)
		{
			perror("execl");
			return -1;
		}
	}
	if((pid = fork()) < 0)
	{
		perror("fork");
		return -1;
	}
	sleep(1);
	if(pid == 0)
	{
		printf("fork2 ok; execv");
		if(execv("/bin/ls", arg) < 0)
		{
			perror("execv");
			return -1;
		}
	}
	sleep(1);
	if((pid = fork()) < 0)
	{
		perror("fork");
		return -1;
	}
	sleep(1);
	if(pid == 0)
	{
		printf("fork3 ok; execlp");
		if(execlp("ls", "ls", "-al", NULL) < 0)
		{
			perror("execlp");
			return -1;
		}
	}
	sleep(1);
	if((pid = fork()) < 0)
	{
		perror("fork");
		return -1;
	}
	sleep(1);
	if(pid == 0)
	{
		printf("fork4 ok; execvp");
		if(execvp("ls", arg) < 0)
		{
			perror("execlp");
			return -1;
		}
	}
	sleep(1);
	if((pid = fork()) < 0)
	{
		perror("fork");
		return -1;
	}
	sleep(1);
	if(pid == 0)
	{
		printf("fork5 ok; execle");
		if(execle("/bin/ls", arg, NULL) < 0)
		{
			perror("execle");
			return -1;
		}
	}
	sleep(1);
	if((pid = fork()) < 0)
	{
		perror("fork");
		return -1;
	}
	sleep(1);
	if(pid == 0)
	{
		printf("fork6 ok; execve");
		if(execve("/bin/ls", arg, NULL) < 0)
		{
			perror("execve");
			return -1;
		}
	}
	sleep(1);

	return 0;
}
