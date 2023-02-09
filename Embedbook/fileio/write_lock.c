#include<unistd.h>
#include<sys/file.h>
#include<sys/stat.h>
#include<stdio.h>
#include<stdlib.h>

int lock_set(int fd,int type);

int main(void)
{
	int fd;

	if((fd = open("hello",O_RDWR)) < 0)
	{
		perror("fail to open");
		return -1;
	}

	lock_set(fd,F_WRLCK);
	getchar();

	lock_set(fd,F_UNLCK);
	getchar();
	
	close(fd);

	return 0;
}
