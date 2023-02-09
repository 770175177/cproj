#include<stdio.h>
#include<string.h>
#include<sys/types.h>
#include<sys/shm.h>
#include<sys/ipc.h>
#include<stdlib.h>
#include<unistd.h>
#include<signal.h>

struct mybuf
{
	int pid;
	char buf[128];
};

void myfun(int signum)
{
	return ;
}

int main(int argc, char *argv[])
{
	int shmid;
	int key;
	struct mybuf *p;
	pid_t pid;	
	
	key = ftok("b.c", 'a');
	if(key < 0)
	{
		printf("create key failure\n");
		return -1;
	}
	printf("create key success\n");
	shmid = shmget(key, 128, IPC_CREAT | 0777);
	if(shmid < 0)
	{
		printf("create share memory failure\n");
		return -2;
	}
	printf("create share memory success, shmid=%d\n", shmid);
		
	signal(SIGUSR1, myfun);
	p = (struct mybuf *)shmat(shmid, NULL, 0);	
	if(p == NULL)
	{
		printf("parent process:shmat function failure\n");
		return -3;
	}
	//get server pid
	pid = p->pid;
	p->pid = getpid();
	kill(pid, SIGUSR2);
	//read and write
	while(1)
	{
		pause();	//wait srver write
		printf("client process recv ,%s", p->buf);
		kill(pid, SIGUSR2);
	}		

	//shmdt(p);	//delete user memory

	shmctl(shmid, IPC_RMID, NULL);
	system("ipcs -m");

	return 0;
}
