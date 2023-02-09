#include<stdio.h>
#include<sys/types.h>
#include<sys/sem.h>
#include<stdlib.h>
#include<unistd.h>
#include<signal.h>

int main(int argc, char *argv[])
{
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
	
	return 0;
}
