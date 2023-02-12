#include <stdio.h>
#include <unistd.h>
#include <errno.h>
#include <time.h>

int read_data(int *);
int write_data(int *);

int main(int argc, const char *argv[])
{
	int pipes[2];
	pid_t pid;
	
	if(pipe(pipes) < 0)
	{
		perror("pipe");
		return -1;
	}
	if((pid = fork()) < 0)
	{
		perror("fork");
		return -1;
	}
	if(pid == 0)
	{
		sleep(5);
		printf("son\n");
		read_data(pipes);
	}
	else
	{
		printf("father\n");
		write_data(pipes);
	}

	return 0;
}

int read_data(int pipes[])
{
	int c;

	close(pipes[1]);
	while((c = read(pipes[0], &c, 1)) > 0)
	{
		putchar(c);
	}

	return 0;
}

int write_data(int pipes[])
{
	int c;

	close(pipes[0]);
	while((c = getchar()) > 0)
	{
		if(write(pipes[1], &c, 1) < 0)
		{
			perror("parent: write");
			close(pipes[1]);
			return -1;
		}
	}
	close(pipes[1]);

	return 0;
}
