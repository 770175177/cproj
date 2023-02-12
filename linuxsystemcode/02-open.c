#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

int main(int argc, const char *argv[])
{
	int fd;
	char *leds = "/dev/leds";
	char *test1 = "/bin/test1";
	char *test2 = "/bin/test2";

	if((fd = open(leds, O_RDWR|O_NOCTTY|O_NDELAY)) < 0)
	{
		printf("open %s failed!\n", leds);
	}
	printf("%s fd is %d\n", leds, fd);

	if((fd = open(test1, O_RDWR, 0777)) < 0)
	{
		printf("open %s failed!\n", test1);
	}
	printf("%s fd is %d\n", test1, fd);

	if((fd = open(leds, O_RDWR|O_CREAT)) < 0)
	{
		printf("open %s failed!\n", test2);
	}
	printf("%s fd is %d\n", test2, fd);

	return 0;
}
