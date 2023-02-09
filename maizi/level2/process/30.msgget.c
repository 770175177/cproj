#include<stdio.h>
#include<sys/types.h>
#include<sys/msg.h>
#include<stdlib.h>
#include<unistd.h>
#include<signal.h>

int main(int argc, char *argv[])
{
	int msgid;
	msgid = msgget(IPC_PRIVATE, 0777);
	if(msgid < 0)
	{
		printf("create message queue failure\n");
		return -1;
	}
	printf("create message queue success, msgid=%d\n", msgid);
	
	system("ipcs -q");
	msgctl(msgid, IPC_RMID, NULL);
	system("ipcs -q");
	
	return 0;
}
