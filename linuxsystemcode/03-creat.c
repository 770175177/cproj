#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

int main(int argc, const char *argv[])
{
	int fd;
	char *test3 = "/bin/test3";

	if((fd = creat(test3, 0777)) < 0)
	{
		printf("creat %s failed\n", test3);
	}
	printf("creat %s, fd is %d\n", test3, fd);

	return 0;
}
