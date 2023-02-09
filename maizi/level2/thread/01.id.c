#include<stdio.h>
#include<pthread.h>
#include<stdlib.h>
#include<string.h>

int main()
{
	pid_t pid;
	pthread_t tid;

	pid = getpid();
	tid = pthread_self();

	printf("pid is %u, tid is %lu\n", pid, tid);

	return 0;
}
