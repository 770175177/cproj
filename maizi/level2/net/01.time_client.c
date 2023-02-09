#include<stdio.h>
#include<string.h>
#include<strings.h>
#include<arpa/inet.h>
#include<sys/types.h>
#include<sys/socket.h>
#include<sys/stat.h>

int main(int argc, char *argv[])
{
	int sockfd;
	struct sockaddr_in server;
	char buf[100];
	int bytes;

	if((sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0)
	{
		perror("socket error");
		return -1;
	}

	bzero(&server, sizeof(server));
	server.sin_family = AF_INET;
	server.sin_addr.s_addr = inet_addr("0.0.0.0");
	server.sin_port = htons(1236);

	if(connect(sockfd, (struct sockaddr*)&server, sizeof(server)) < 0)
	{
		perror("connect error");
		return -2;
	}

	bzero(buf, sizeof(buf));
	bytes = read(sockfd, buf, 100);
	if(bytes < 0)
	{
		printf("read error\n");
		return -3;
	}
	if(0 == bytes)
	{
		printf("server close connection\n");
		return -4;
	}

	printf("read bytes %d\n", bytes);
	printf("time: %s", buf);

	close(sockfd);	
	return 0;
}
