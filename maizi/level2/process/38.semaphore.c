#include<stdio.h>
#include<stdlib.h>
#include<pthread.h>
#include<semaphore.h>

sem_t sem;

void *fun(void *var)
{
	int j;
	sem_wait(&sem);		//p
	for(j=0; j<10; j++)
	{
		usleep(100);
		printf("this is fun, j=%d\n", j);
	}
}

int main(int argc, char *argv[])
{
	int i;
	char str[] = "hello linux";
	int ret;
	pthread_t tid;

	sem_init(&sem, 0, 0);
	ret = pthread_create(&tid, NULL, fun, (void *)str);
	if(ret < 0)
	{
		printf("create thread failure\n");
		return -1;
	}
	for(i=0; i<10; i++)
	{
		usleep(100);
		printf("this is main fun, i=%d\n", i);
	}
	sem_post(&sem);		//v
	while(1);

/*
	int semid;
	semid = semget(IPC_PRIVATE, 3, 0777);
	if(semid < 0)
	{
		printf("create semaphore lamp failure\n");
		return -1;
	}
	printf("create semaphore lamp success, semid=%d\n", semid);
	system("ipcs -s");
	semctl(semid, 0, IPC_RMID);
	system("ipcs -s");
	*/
	return 0;
}
