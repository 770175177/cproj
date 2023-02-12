#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>

#define BUZZER_C 2

int main(int argc, const char *argv[])
{
	int fd, ret;
	char *buzzer = "/dev/buzzer_ctl";

	printf("argv1 is cmd, argv2 is io\n");
	
	if(atoi(argv[1]) >= BUZZER_C)
	{
		printf("argv[1] is 0 or 1\n");
		exit(-1);
	}

	if((fd = open(buzzer, O_RDWR|O_NOCTTY|O_NDELAY, 0777)) < 0)
	{
		printf("open %s failed!\n", buzzer);
	}
	printf("%s fd is %d\n", buzzer, fd);
	
	ret = ioctl(fd, atoi(argv[1]), atoi(argv[2]));
	printf("ioctl %s success\n", buzzer);

	close(fd);

	return 0;
}
