#include<stdio.h>
#include<pthread.h>
#include<stdlib.h>
int thread_inter = 0;
void *fun(void *var)
{
	int j;
	while(thread_inter == 0);
	for(j=0; j<10; j++)
	{
		usleep(100);
		printf("this is fun j=%d\n", j);
	}
	printf("var is :%s\n", (char *)var);
}

int main()
{
	int i;
	char str[] = "hello linux";
	pthread_t tid;
	int ret;

	ret = pthread_create(&tid, NULL, fun, (void *)str);
	if(ret < 0)
	{
		printf("create thread failure\n");
		return -1;
	}
	for(i=0; i<10; i++)
	{
		usleep(100);
		printf("this is main fun i=%d\n", i);
	}
	thread_inter = 1;
	while(1);

	return 0;
}
