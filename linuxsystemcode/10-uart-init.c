#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <errno.h>
#include <termios.h>
#include <unistd.h>
#include <string.h>

int set_opt(int fd, int nSpeed, int bBits, char nEvent, int nStop);

int main(int argc, const char *argv[])
{
	int fd, i=10, wr_static;
	char *uart3 = "/dev/ttySAC3";
	char *buffer = "hello world\n";

	printf("\r\niTop4412 uart3 write test start!\r\n");

	if((fd = open(uart3, O_RDWR|O_NOCTTY|O_NDELAY, 0777)) < 0)
	{
		printf("open %s failed!\n", uart3);
	}
	printf("open %s is success!\n", uart3);
	set_opt(fd, 115200, 8, 'N', 1);

	while(i--)
	{
		wr_static = write(fd, buffer, strlen(buffer));
		if(wr_static < 0)
		{
			printf("write failed\n");
		}
		else
		{
			printf("write_static is %d\n",wr_static);
		}
		sleep(1);
	}
	
	close(fd);

	return 0;
}

int set_opt(int fd, int nSpeed, int nBits, char nEvent, int nStop)
{
	struct termios newtio, oldtio;
	if(tcgetattr(fd, &oldtio) != 0)
	{
		perror("Setup Serial 1");
		return -1;
	}
	bzero(&newtio, sizeof(newtio));
	newtio.c_cflag |= CLOCAL | CREAD;
	newtio.c_cflag &= ~CSIZE;

	switch(nBits)
	{
		case 7:
			newtio.c_cflag |= CS7;
			break;
		case 8:
			newtio.c_cflag |= CS8;
			break;
	}

	switch(nEvent)
	{
		case 'O':
			newtio.c_cflag |= PARENB;
			newtio.c_cflag |= PARODD;
			newtio.c_iflag |= (INPCK | ISTRIP);
			break;
		case 'E':
			newtio.c_iflag |= (INPCK | ISTRIP);
			newtio.c_cflag |= PARENB;
			newtio.c_cflag &= ~PARODD;
			break;
		case 'N':
			newtio.c_cflag &= ~PARENB;
			break;
	}

	switch(nSpeed)
	{
		case 2400:
			cfsetispeed(&newtio, B2400);
			cfsetospeed(&newtio, B2400);
			break;
		case 4800:
			cfsetispeed(&newtio, B4800);
			cfsetospeed(&newtio, B4800);
			break;
		case 9600:
			cfsetispeed(&newtio, B9600);
			cfsetospeed(&newtio, B9600);
			break;
		case 115200:
			cfsetispeed(&newtio, B115200);
			cfsetospeed(&newtio, B115200);
			break;
		case 460800:
			cfsetispeed(&newtio, B460800);
			cfsetospeed(&newtio, B460800);
			break;
		default:
			cfsetispeed(&newtio, B9600);
			cfsetospeed(&newtio, B9600);
			break;
	}

	if(nStop == 1)
	{
		newtio.c_cflag &= ~CSTOPB;
	}
	else if(nStop == 2){
		newtio.c_cflag |= CSTOPB;
	}
	newtio.c_cc[VTIME] = 0;
	newtio.c_cc[VMIN] = 0;
	tcflush(fd, TCIFLUSH);

	if((tcsetattr(fd, TCSANOW, &newtio)) != 0)
	{
		perror("com set error");
		return -1;
	}

	return 0;
}
