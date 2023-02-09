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
	struct msgbuf sendbuf, recvbuf;
	msgid = msgget(IPC_PRIVATE, 0777);
	if(msgid < 0)
	{
		printf("create message queue failure\n");
		return -1;
	}
	printf("create message queue success, msgid=%d\n", msgid);
	//init
	sendbuf.type = 100;
	printf("please input message:\n");
	fgets(sendbuf.voltage, 124, stdin);
	//write
	msgsnd(msgid, (void *)&sendbuf, strlen(sendbuf.voltage), 0);
	//read
	memset(recvbuf.voltage, 0, 124);
	readret = msgrcv(msgid, (void *)&recvbuf, 124, 100, 0);
	printf("recv: %s", recvbuf.voltage);	
	printf("readret=%d\n", readret);
	//second read
	msgrcv(msgid, (void *)&recvbuf, 124, 100, 0);
	printf("second read: %s", recvbuf.voltage);	

	system("ipcs -q");
	msgctl(msgid, IPC_RMID, NULL);
	system("ipcs -q");
	
	return 0;
}
