#include<stdio.h>
#include<pthread.h>
#include<stdlib.h>
#include<string.h>
#include<unistd.h>
#include<sys/types.h>

struct student{
	int age;
	char name[20];
};

void *thread_fun(void *i)
{
	while(1)
	{
		if(*(int *)i%2 == 1)
		{
			printf("new thread: %d\n", *(int *)i);
			(*(int *)i)++;
		}
	}	
	return (void *)0;
}

int main(int argc, char *argv[])
{
	pthread_t tid;
	int ret;
	int i=0;
	struct student stu;
	stu.age = 20;
	memcpy(stu.name, "liming", 20); 

	ret = pthread_create(&tid, NULL, thread_fun, (void *)(&i));
	if(ret < 0)
	{
		printf("create new thread failure\n");
		return -1;
	}
	while(1)
	{
		if(i%2 == 0)
		{
			printf("main thread: %d\n", i);
			i++;
		}
	}

	pthread_exit(0);
}
