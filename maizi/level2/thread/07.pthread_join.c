#include<stdio.h>
#include<pthread.h>
#include<stdlib.h>
#include<string.h>
#include<unistd.h>
#include<sys/types.h>

void *fun1(void *arg)
{
	printf("I am thread 1\n");
	return (void *)1;
}
void *fun2(void *arg)
{
	printf("I am thread 2\n");
	pthread_detach(pthread_self());
	pthread_exit((void *)2);
}

int main(int argc, char *argv[])
{
	pthread_t tid1, tid2;
	int ret1, ret2;
	void *rval1, *rval2;

	ret1 = pthread_create(&tid1, NULL, fun1, NULL);
	if(ret1 < 0)
	{
		printf("create new thread 1 failure\n");
		return -1;
	}
	ret2 = pthread_create(&tid2, NULL, fun2, NULL);
	if(ret2 < 0)
	{
		printf("create new thread 2 failure\n");
		return -2;
	}
	printf("main thread\n");

	printf("join rval 1: %d\n", pthread_join(tid1, &rval1));
	printf("join rval 2: %d\n", pthread_join(tid2, &rval2));

	printf("thread 1 exit code is %d\n", *(int *)rval1);
	printf("thread 2 exit code is %d\n", *(int *)rval2);
	printf("oh, I'm main thread\n");
	
	return 0;
}
