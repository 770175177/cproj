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
	char *adc = "/dev/adc";
	char buf[512];

	memset(buf, 0, sizeof(buf));
	printf("adc ready!\n");

	if((fd = open(adc, O_RDWR|O_NOCTTY|O_NDELAY, 0777)) < 0)
	{
		printf("open %s failed!\n", adc);
	}
	printf("%s fd is %d\n", adc, fd);
	
	len = read(fd, buf, 10);
	if(len == 0)
	{
		printf("return null\n");
	}
	r = atoi(buf);
	r = (int)(r * 10000 / 4095);
	printf("ret value is %d\n", r);
	
	close(fd);

	return 0;
}
