#include<stdio.h>
#include<string.h>
#include<strings.h>
#include<arpa/inet.h>
#include<sys/types.h>
#include<sys/socket.h>
#include<sys/stat.h>
#include<time.h>

int main(int argc, char *argv[])
{
	int sockfd;
	struct sockaddr_in server;
	char buf[100] = "Hello UDP!\n";
	char read_buf[100] = {0};
	int length, rv, len;

	sockfd = socket(AF_INET, SOCK_DGRAM, 0);
	if(sockfd < 0)
	{
		perror("create socket failed");
		return -1;
	}
	printf("create socket success\n");
	bzero(&server, sizeof(server));
	server.sin_family = AF_INET;
	server.sin_port = htons(1234);
	server.sin_addr.s_addr = inet_addr("127.0.0.1");
	
	length = sizeof(struct sockaddr);
	len = sizeof(struct sockaddr);
	sendto(sockfd, buf, strlen(buf), 0, (struct sockaddr *)&server, length);
	printf("len:%d\n", length);
	rv = recvfrom(sockfd, read_buf, 100, 0, (struct sockaddr *)&server, &len);
	if(rv < 0)
	{
		printf("recvfrom error\n");
		close(sockfd);
		return -1;
	}	
	printf("buf: %s\n", read_buf);
	close(sockfd);

	return 0;
}
