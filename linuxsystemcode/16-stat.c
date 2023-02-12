#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <fcntl.h>


int main(int argc, const char *argv[])
{
	struct stat groupstat;
	int fd, ret;

	if(argc < 2)
	{
		printf("please input file path\n");
		return 0;
	}
	ret = stat(argv[1], &groupstat);	
	if(ret < 0)
	{
		printf("check filepath\n");
		return 0;
	}
	printf("stat function test, %s of st_ino is %ld\n", argv[1], groupstat.st_ino);

	fd = open(argv[1], O_RDWR|O_NOCTTY|O_NDELAY);
	if(fd < 0)
	{
		printf("open file failed\n");
		return 0;
	}
	ret = fstat(fd, &groupstat);
	if(ret < 0)
	{
		printf("check filepath\n");
		return 0;
	}
	printf("fstat function test, %s of st_ino is %ld\n", argv[1], groupstat.st_ino);

	ret = lstat(argv[1], &groupstat);
	if(ret < 0)
	{
		printf("check filepath\n");
		return 0;
	}
	printf("lstat function test, %s of st_ino is %ld\n", argv[1], groupstat.st_ino);
	
	return 0;
}
