#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <errno.h>
#include <unistd.h>
#include <string.h>

#define SIZE 1024

int main(int argc, const char *argv[])
{
	int fds, fdt;
	char buf[SIZE];
	char *fileold, *filenew;

	if(argc < 3)
	{
		printf("please input file path\n");
		return -1;
	}
	fileold = argv[1];
	filenew = argv[2];
	if((fds = open(fileold, O_RDONLY)) < 0)
	{
		perror("open");
		return -1;
	}
	if((fdt = open(filenew, O_WRONLY|O_CREAT)) < 0)
	{
		perror("open");
		return -1;
	}
	
	while(read(fds, buf, SIZE))
	{
		write(fdt, buf, strlen(buf));
	}
	
	close(fds);
	close(fdt);

	printf("copy %s to %s success!\n", fileold, filenew);

	return 0;
}
