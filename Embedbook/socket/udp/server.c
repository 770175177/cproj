#include<stdio.h>
#include<sys/types.h>
#include<sys/socket.h>
#include<stdlib.h>
#include<string.h>
#include<unistd.h>
#include<netinet/in.h>
#include<arpa/inet.h>

#define BUFFER_SIZE 128

int main(int argc, char * argv[])
{
	int sockfd;
	struct sockaddr_in servaddr, cliaddr;
	socklen_t peerlen;
	char buf[BUFFER_SIZE];

	if(argc < 3)
	{
		printf("Usage : %s <ip> <port>\n",argv[0]);
		exit(-1);
	}

	//create socket
	if((sockfd = socket(AF_INET, SOCK_DGRAM, 0))  == -1)
	{
		perror("socket");
		exit(-1);
	}
	printf("sockfd = %d\n",sockfd);

	//set param of sockaddr_in
	bzero(&servaddr, sizeof(servaddr));
	servaddr.sin_family = AF_INET;
	servaddr.sin_port = htons(atoi(argv[2]));
	servaddr.sin_addr.s_addr = inet_addr(argv[1]);

	//bind
	if(bind(sockfd, (struct sockaddr *)&servaddr, sizeof(servaddr)) < 0)
	{
		perror("bind");
		exit(-1);
	}
	printf("bind success!\n");

	//wait for connect
	peerlen = sizeof(cliaddr);
	while(1)
	{
		if(recvfrom(sockfd, buf, sizeof(buf), 0, (struct sockaddr *)&cliaddr, &peerlen) < 0)
		{
			perror("recvfrom");
			exit(-1);
		}
		printf("Receive a message : %s\n",buf);
		strcpy(buf,"Welcome to server");
		sendto(sockfd, buf, sizeof(buf), 0, (struct sockaddr *)&cliaddr, peerlen);
	}
	close(sockfd);
	exit(0);
}
