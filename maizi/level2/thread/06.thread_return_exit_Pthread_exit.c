#include<stdio.h>
#include<pthread.h>
#include<stdlib.h>
#include<string.h>
#include<unistd.h>
#include<sys/types.h>

void *thread_fun(void *arg)
{
	if(strcmp("1", (char *)arg) == 0)
	{
		printf("new thread return!\n");
		return (void *)1;
	}
	if(strcmp("2", (char *)arg) == 0)
	{
		printf("new thread pthread_exit!\n");
		pthread_exit((void *)2);
	}
	if(strcmp("3", (char *)arg) == 0)
	{
		printf("new thread exit!\n");
		exit(3);
	}
}

int main(int argc, char *argv[])
{
	pthread_t tid;
	int ret;

	ret = pthread_create(&tid, NULL, thread_fun, (void *)argv[1]);
	if(ret < 0)
	{
		printf("create new thread failure\n");
		return -1;
	}
	sleep(1);
	printf("main thread\n");

	return 0;
}
