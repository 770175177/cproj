#include<stdio.h>
#include<stdlib.h>
#include<pthread.h>
#include<sys/types.h>
#include<sys/ipc.h>
#include<sys/sem.h>

union semun
{
	int val;
	struct semid_ds *buf;
	unsigned short *array;
	struct seminfo *__buf;
};

int semid;
union semun mysemun;
struct sembuf mysembuf;

int main(int argc, char *argv[])
{
	int i;
	int key;
	
	key = ftok("d.c", 'a');
	if(key < 0)
	{
		printf("create key failure\n");
		return -1;
	}
	printf("create key success\n");
	semid = semget(key, 3, IPC_CREAT|0777);
	if(semid < 0)
	{
		printf("create semaphore failure\n");
		return -2;
	}
	printf("create semaphore success, semid=%d\n", semid);
	//init sem
	//mysemun.val = 0;
	//semctl(semid, 0, SETVAL, mysemun);
	mysembuf.sem_num = 0;
	mysembuf.sem_flg = 0;
	for(i=0; i<10; i++)
	{
		usleep(100);
		printf("this is main fun, i=%d\n", i);
	}
	//sem_post(&sem);		//v
	mysembuf.sem_op = 1;
	semop(semid, &mysembuf, 1);
	while(1);

	return 0;
}
