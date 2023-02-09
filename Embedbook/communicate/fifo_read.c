#include<stdio.h>
#include<stdlib.h>
#include<errno.h>
#include<fcntl.h>
#include<string.h>
#include<unistd.h>
#include<sys/types.h>
#include<sys/stat.h>

#define MYFIFO "/tmp/myfifo"

int main()
{
	char buf[256];
	int fd;
	int nread;

	if(access(MYFIFO,F_OK) == -1)
	{
		if(mkfifo(MYFIFO,0666) < 0)
		{
			perror("fail to mkfifo");
			exit(-1);
		}
	}

	if((fd = open(MYFIFO,O_RDONLY)) < 0)
	{
		perror("fail to open fifo");
		exit(-1);
	}

	while(1)
	{
		memset(buf,0,sizeof(buf));
		if((nread = read(fd,buf,256)) > 0)
		{
			printf("Read '%s' from FIFO\n",buf);
		}
	}
	close(fd);
	return 0;
}
