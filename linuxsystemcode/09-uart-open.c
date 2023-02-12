#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>

int main(int argc, const char *argv[])
{
	int fd, len, r = 0;
	char *uart3 = "/dev/ttySAC3";

	if((fd = open(uart3, O_RDWR|O_NOCTTY|O_NDELAY, 0777)) < 0)
	{
		printf("open %s failed!\n", uart3);
	}
	printf("%s fd is %d\n", uart3, fd);
	
	close(fd);

	return 0;
}
