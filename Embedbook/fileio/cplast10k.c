#include<stdio.h>
#include<sys/stat.h>
#include<fcntl.h>
#include<stdlib.h>
#include<unistd.h>
#include<string.h>

#define BUFFER_SIZE 1024
#define SRC_FILE_NAME "src_file"
#define DEST_FILE_NAME "dest_file"
#define OFFSET 10240

int main()
{
	int fm,ft;
	unsigned char buff[BUFFER_SIZE];
	int read_len;
	memset(buff,0,sizeof(buff));

	if((fm = open(SRC_FILE_NAME,O_RDONLY)) < 0)
	{
		perror("fail to open src_file");
		return -1;
	}

	if((ft = open(DEST_FILE_NAME,O_WRONLY|O_CREAT|O_TRUNC,0644)) < 0)
	{
		perror("fail to open dest_file");
		return -1;
	}
	
	lseek(fm,-OFFSET,SEEK_END);

	while((read_len = read(fm, buff,sizeof(buff))) > 0)
	{
		write(ft,buff,read_len);
	}
	
	close(fm);
	close(ft);
}
