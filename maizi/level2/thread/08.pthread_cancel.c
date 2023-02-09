#include<stdio.h>
#include<pthread.h>
#include<stdlib.h>
#include<string.h>
#include<unistd.h>
#include<sys/types.h>

void *thread_fun(void *arg)
{
	int stateval;
	stateval = pthread_setcancelstate(PTHREAD_CANCEL_DISABLE, NULL);
	if(stateval < 0)
	{
		printf("set cancel state failed\n");
	}
	printf("I'm new thread\n");
	sleep(4);
	printf("about to cancel\n");
	stateval = pthread_setcancelstate(PTHREAD_CANCEL_DISABLE, NULL);
	if(stateval < 0)
	{
		printf("set cancel state failed\n");
	}	
	printf("first cancel point\n");
	printf("second cancel point\n");

	return (void *)20;
}

int main(int argc, char *argv[])
{
	pthread_t tid;
	int ret, cval, jval;
	void *rval;

	ret = pthread_create(&tid, NULL, thread_fun, NULL);
	if(ret < 0)
	{
		printf("create new thread failure\n");
		return -1;
	}
	printf("create new thread success\n");
	sleep(2);

	cval = pthread_cancel(tid);
	if(cval < 0)
	{
		printf("cancel thread failure\n");
	}
	jval = pthread_join(tid, &rval);

	printf("new thread exit code is: %d\n", (int *)rval);
	
	return 0;
}
