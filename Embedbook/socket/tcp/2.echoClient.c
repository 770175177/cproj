#include<stdio.h>
#include<sys/types.h>
#include<sys/socket.h>
#include<stdlib.h>
#include<string.h>
#include<unistd.h>
#include<netinet/in.h>
#include<arpa/inet.h>

#define BUFFER_SIZE 128

int main(int argc,char * argv[])
{
	int sockfd;
	char buf[BUFFER_SIZE];
	struct sockaddr_in servaddr;

	if(argc < 3)
	{
		printf("Usage: %s <ip> <port>",argv[0]);
		exit(-1);
	}
	
	//create socket connection
	if((sockfd = socket(AF_INET, SOCK_STREAM, 0)) == -1)
	{
		perror("socket");
		exit(-1);
	}

	//set param of sockaddr_in
	bzero(&servaddr, sizeof(servaddr));
	servaddr.sin_family = AF_INET;
	servaddr.sin_port = htons(atoi(argv[2]));
	servaddr.sin_addr.s_addr = inet_addr(argv[1]);

	//connect
	if(connect(sockfd, (struct sockaddr *)&servaddr, sizeof(servaddr)) == -1)
	{
		perror("connect");
		exit(-1);
	}

	while(1)
	{
		//send msg to server
		printf("input >");
		fgets(buf, sizeof(buf), stdin);
		send(sockfd, buf, sizeof(buf), 0);
		if(strcmp(buf, "quit\n") == 0)break;

		//recv msg from server
		bzero(buf,sizeof(buf));
		recv(sockfd, buf, sizeof(buf), 0);
		printf("recv from server : %s\n", buf);
	}
	printf("client exit\n");
	close(sockfd);
	exit(0);
}
