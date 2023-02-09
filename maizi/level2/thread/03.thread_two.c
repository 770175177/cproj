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

void *thread_fun(void *stu)
{
	printf("student age is %d, name is %s\n", ((struct student*)stu)->age, ((struct student *)stu)->name);
	return (void *)0;
}

int main(int argc, char *argv[])
{
	pthread_t tid;
	int ret;
	int i;
	struct student stu;
	stu.age = 20;
	memcpy(stu.name, "liming", 20); 

	ret = pthread_create(&tid, NULL, thread_fun, (void *)(&stu));
	if(ret < 0)
	{
		printf("create new thread failure\n");
		return -1;
	}
	for(i=0; i<argc; i++)
	{
		printf("main thread arg[%d] is %s\n", i, argv[i]);
	}
	sleep(1);

	return 0;
}
