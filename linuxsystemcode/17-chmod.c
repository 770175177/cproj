#include <stdio.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <fcntl.h>

int main(int argc, const char *argv[])
{
	int fd;
	int mode;

	if(argc < 4)
	{
		printf("usage: %s <filepath> <mode>\n", argv[0]);
		return -1;
	}
	mode = atoi(argv[2]);
	if(chmod(argv[1], mode))
	{
		printf("please check filepath\n");
		return -1;
	}
	printf("chmod %s is %d\n", argv[1], mode);

	if((fd = open(argv[3], O_RDWR|O_NOCTTY|O_NDELAY)) < 0)
	{
		printf("open file failed\n");
		return -1;
	}
	if(fchmod(fd, mode) < 0)
	{
		printf("please check filepath\n");
		return -1;
	}
	printf("chmod %s is %d\n", argv[1], mode);
	close(fd);

	return 0;
}
