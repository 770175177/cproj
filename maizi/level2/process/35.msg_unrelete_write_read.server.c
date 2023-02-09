#include<stdio.h>
#include<sys/types.h>
#include<sys/msg.h>
#include<stdlib.h>
#include<unistd.h>
#include<signal.h>
#include<string.h>

struct msgbuf
{
	long type;
	char voltage[124];
	char ID[4];
};

int main(int argc, char *argv[])
{
	int msgid;
	int readret;
	int key;
	struct msgbuf sendbuf, recvbuf;
	pid_t pid;

	key = ftok("c.c", 'a');
	if(key < 0)
	{
		printf("create key failure\n");
		return -1;
	}
	msgid = msgget(key, IPC_CREAT|0777);
	if(msgid < 0)
	{
		printf("create message queue failure\n");
		return -2;
	}
	printf("create message queue success, msgid=%d\n", msgid);
	system("ipcs -q");
	
	pid = fork();
	if(pid > 0)
	{
		sendbuf.type = 100;
		while(1)
		{
			memset(sendbuf.voltage, 0, 124);
			printf("please input message:\n");
			fgets(sendbuf.voltage, 124, stdin);
			//write
			msgsnd(msgid, (void *)&sendbuf, strlen(sendbuf.voltage), 0);
		}
	}
	if(pid == 0)//child	
	{
		while(1)
		{
			memset(recvbuf.voltage, 0, 124);
			msgrcv(msgid, (void *)&recvbuf, 124, 200, 0);
			printf("recv message from queue:%s", recvbuf.voltage);
		}
	}
	msgctl(msgid, IPC_RMID, NULL);
	system("ipcs -q");
	
	return 0;
}
