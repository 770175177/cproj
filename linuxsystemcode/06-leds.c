#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>

#define LED_NUM 2
#define LED_C 2

int main(int argc, const char *argv[])
{
	int fd;
	char *leds = "/dev/leds";

	// cmd=0: off, cmd=1: on
	// io=0: led0, io=1: led1
	printf("argv1 is cmd, argv2 is io\n");
	
	if(atoi(argv[1]) > LED_C)
	{
		printf("argv[1] is 0 or 1\n");
		exit(-1);
	}
	if(atoi(argv[2]) > LED_NUM)
	{
		printf("argv[2] is 0 or 1\n");
		exit(-1);
	}

	if((fd = open(leds, O_RDWR|O_NOCTTY|O_NDELAY, 0777)) < 0)
	{
		printf("open %s failed!\n", leds);
	}
	printf("%s fd is %d\n", leds, fd);
	
	ioctl(fd, atoi(argv[1]), atoi(argv[2]));
	printf("ioctl %s success\n", leds);

	close(fd);

	return 0;
}
