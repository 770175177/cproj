#include <stdio.h>
#include <unistd.h>
#include <errno.h>

int main(int argc, const char *argv[])
{
	if(argc < 2)
	{
		printf("please input fine path\n");
		return -1;
	}
	if(rmdir(argv[1]) < 0)
	{
		perror("rmdir");
		return -1;
	}
	printf("rmdir %s success\n", argv[1]);

	return 0;
}
