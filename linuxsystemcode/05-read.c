#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>

#define MAX_LEN 1000

int main(int argc, const char *argv[])
{
	int fd, length_wr, length_rd, ret;
	char *test_write = "/bin/test_write";
	char buf[] = "Hello Write Function!";
	char buf_rd[MAX_LEN];

	if((fd = open(test_write, O_RDWR|O_CREAT, 0777)) < 0)
	{
		printf("open %s failed!\n", test_write);
	}
	printf("%s fd is %d\n", test_write, fd);
	
	length_wr = write(fd, buf, strlen(buf));
	if(length_wr == -1)
	{
		perror("write");
	}
	printf("Write Function OK, Write %d Bytes!\n", length_wr);

	close(fd);

	if((fd = open(test_write, O_RDONLY, 0777)) < 0)
	{
		printf("open %s failed!\n", test_write);
	}
	printf("%s fd is %d\n", test_write, fd);
	
	if((ret = read(fd, buf_rd, MAX_LEN)) < 0)
	{
		perror("read");
	}
	printf("Read %d Bytes: %s\n", ret, buf_rd);

	return 0;
}
