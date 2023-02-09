#include<stdio.h>
#include<sys/types.h>
#include<sys/shm.h>
#include<sys/ipc.h>
#include<stdlib.h>
#include<unistd.h>
#include<signal.h>

int main(int argc, char *argv[])
{
	int shmid;
	int key;
	key = ftok("./b.c", 'b');
	if(key < 0)
	{
		printf("create key failure\n");
		return -1;
	}
	printf("create key success, key=%d\n", key);
	shmid = shmget(key, 128, IPC_CREAT | 0777);
	if(shmid < 0)
	{
		printf("create share memory failure\n");
		return -2;
	}
	printf("create share memory success, shmid=%d\n", shmid);
	
	system("ipcs -m");
//	system("ipcrm -m shmid");

	return 0;
}
