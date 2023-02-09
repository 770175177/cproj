#include<stdio.h>
#include<string.h>
#include<sys/types.h>
#include<sys/shm.h>
#include<sys/ipc.h>
#include<stdlib.h>
#include<unistd.h>
#include<signal.h>

void myfun(int signum)
{
	return ;
}

int main(int argc, char *argv[])
{
	int shmid;
	int key;
	char *p;
	pid_t pid;	

	shmid = shmget(IPC_PRIVATE, 128, IPC_CREAT | 0777);
	if(shmid < 0)
	{
		printf("create share memory failure\n");
		return -2;
	}
	printf("create share memory success, shmid=%d\n", shmid);
	
	pid = fork();
	if(pid > 0)
	{
		signal(SIGUSR2, myfun);
		p = (char *)shmat(shmid, NULL, 0);	
		if(p == NULL)
		{
			printf("parent process:shmat function failure\n");
			return -3;
		}
		while(1)
		{
			printf("parent write\n");
			fgets(p, 128, stdin);//write share memory
			kill(pid, SIGUSR1);//wait child read
			pause();
		}		
	}
	if(pid == 0)
	{
		signal(SIGUSR1, myfun);
		p = (char *)shmat(shmid, NULL, 0);	
		if(p == NULL)
		{
			printf("child process:shmat function failure\n");
			return -4;
		}
		while(1)
		{
			pause();
			printf("share memory data:%s", p);
			kill(getppid(), SIGUSR2);
		}		
	}

	printf("share memory date:%s\n", p);

	shmdt(p);	//delete user memory

	shmctl(shmid, IPC_RMID, NULL);
	system("ipcs -m");

	return 0;
}
