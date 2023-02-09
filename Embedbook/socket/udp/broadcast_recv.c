#include<stdio.h>
#include<sys/types.h>
#include<sys/socket.h>
#include<stdlib.h>
#include<string.h>
#include<unistd.h>
#include<netinet/in.h>
#include<arpa/inet.h>

#define N 128

int main(int argc, char * argv[])
{
	int sockfd;
	struct sockaddr_in peeraddr, myaddr;
	socklen_t peerlen = sizeof(peeraddr);
	char buf[N];

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

	//set param of sockaddr_in
	bzero(&myaddr, sizeof(myaddr));
	myaddr.sin_family = AF_INET;
	myaddr.sin_port = htons(atoi(argv[2]));
	myaddr.sin_addr.s_addr = inet_addr(argv[1]);

	//bind
	if(bind(sockfd, (struct sockaddr *)&myaddr, sizeof(myaddr)) < 0)
	{
		perror("bind");
		exit(-1);
	}

	while(1)
	{
		recvfrom(sockfd, buf, N, 0, (struct sockaddr *)&peeraddr, &peerlen);
		printf("[%s:%d] %s\n",inet_ntoa(peeraddr.sin_addr),ntohs(peeraddr.sin_port), buf);		
	}
	return (0);
}
