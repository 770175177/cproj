#include<stdio.h>
#include<string.h>
#include<sys/types.h>
#include<sys/shm.h>
#include<sys/ipc.h>
#include<stdlib.h>
#include<unistd.h>
#include<signal.h>

int main(int argc, char *argv[])
{
	int shmid;	

	if(argc < 3)
	{
		printf("please input param\n");
		return -1;
	}	
	if(strcmp(argv[1], "-m") == 0)
		printf("delete share memory\n");
	else
		return -2;
	shmid = atoi(argv[2]);
	printf("shmid = %d\n", shmid);
	shmctl(shmid, IPC_RMID, NULL);		
	system("ipcs -m");

	return 0;
}
