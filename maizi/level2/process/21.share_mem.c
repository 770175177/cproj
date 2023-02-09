#include<stdio.h>
#include<sys/types.h>
#include<sys/shm.h>
#include<stdlib.h>
#include<unistd.h>
#include<signal.h>

int main(int argc, char *argv[])
{
	int shmid;
	shmid = shmget(IPC_PRIVATE, 128, 0777);
	if(shmid < 0)
	{
		printf("create share memory failure\n");
		return -1;
	}
	printf("create share memory success, shmid=%d\n", shmid);
	
	system("ipcs -m");
//	system("ipcrm -m shmid");

	return 0;
}
