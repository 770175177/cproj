#include<unistd.h>
#include<stdio.h>
#include<stdlib.h>

int main()
{
	int fd[2];
	int ret;
	char write_buf[] = "hello linux";
	char read_buf[128]={0};
	
	ret = pipe(fd);
	if(ret < 0)
	{
		printf("create pipe failure\n");
		return -1;
	}
	printf("create pipe success fd[0]=%d, fd[1]=%d\n", fd[0], fd[1]);

	write(fd[1], write_buf, sizeof(write_buf));
	read(fd[0], read_buf, 128);	
	printf("read_buf=%s\n", read_buf);

	close(fd[0]);
	close(fd[1]);
	return 0;
}
