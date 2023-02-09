#include<unistd.h>
#include<stdio.h>
#include<stdlib.h>

int main()
{
	int fd[2];
	int ret;
	char write_buf[] = "hello linux!!!!";
	char read_buf[128]={0};
	int i;
	
	ret = pipe(fd);
	if(ret < 0)
	{
		printf("create pipe failure\n");
		return -1;
	}
	printf("create pipe success fd[0]=%d, fd[1]=%d\n", fd[0], fd[1]);
	while(i<5500)
	{
		printf("all write %d\n", i*sizeof(write_buf));
		write(fd[1], write_buf, sizeof(write_buf));
		printf("write pipe, i=%d\n", i);
		i++;
	}
	
	close(fd[0]);
	close(fd[1]);
	return 0;
}
