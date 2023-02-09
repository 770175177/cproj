#include<stdio.h>
#include<stdlib.h>
#include<errno.h>
#include<fcntl.h>
#include<string.h>
#include<unistd.h>

#define MYFIFO "/tmp/myfifo"

int main(int argc,char * argv[])
{
	int fd;
	int nwrite;

	if(argc < 2)
	{
		printf("Usage : ./fifo_write string\n");
		exit(-1);
	}

	if((fd = open(MYFIFO,O_WRONLY)) < 0)
	{
		perror("fail to open fifo");
		exit(-1);
	}

	if((nwrite = write(fd,argv[1],strlen(argv[1]) + 1)) > 0)
	{
		printf("Write '%s' to FIFO\n",argv[1]);
	}
	close(fd);
	return 0;
}
