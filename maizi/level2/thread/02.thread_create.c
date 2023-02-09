#include<stdio.h>
#include<pthread.h>
#include<stdlib.h>
#include<string.h>
#include<unistd.h>
#include<sys/types.h>

void print_id(char *s)
{
	pid_t pid;
	pthread_t tid;

	pid = getpid();
	tid = pthread_self();

	printf("%s: pid is %u, tid is %lu\n", s, pid, tid);
}

void *thread_fun(void *arg)
{
	print_id(arg);
	return (void *)0;
}

int main()
{
	pthread_t tid;
	int ret;

	ret = pthread_create(&tid, NULL, thread_fun, "new thread");
	if(ret < 0)
	{
		printf("create new thread failure\n");
		return -1;
	}
	print_id("main thread");
	sleep(2);

	return 0;
}
