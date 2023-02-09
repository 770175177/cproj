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
	int listenfd, connfd;
	struct sockaddr_in servaddr, chiaddr;
	socklen_t peerlen;
	char buf[BUFFER_SIZE];

	if(argc < 3)
	{
		printf("Usage: %s <ip> <port>",argv[0]);
		exit(-1);
	}
	
	//create socket connection
	if((listenfd = socket(AF_INET, SOCK_STREAM, 0)) == -1)
	{
		perror("socket");
		exit(-1);
	}
	printf("listenfd = %d\n", listenfd);

	//set param of sockaddr_in
	bzero(&servaddr, sizeof(servaddr));
	servaddr.sin_family = AF_INET;
	servaddr.sin_port = htons(atoi(argv[2]));
	servaddr.sin_addr.s_addr = inet_addr(argv[1]);

	//bing
	if(bind(listenfd, (struct sockaddr *)&servaddr, sizeof(servaddr)) < 0)
	{
		perror("bind");
		exit(-1);
	}
	printf("bind success!\n");

	//listen
	if(listen(listenfd, 10) == -1)
	{
		perror("listen");
		exit(-1);
	}
	printf("Listening...\n");

	//use accept wait for client to connect
	peerlen = sizeof(chiaddr);
	while(1)
	{
		if((connfd = accept(listenfd, (struct sockaddr *)&chiaddr, &peerlen)) < 0)
		{
			perror("accept");
			exit(-1);
		}

		//recv msg
		memset(buf, 0, sizeof(buf));
		if((recv(connfd, buf, BUFFER_SIZE, 0)) == -1)
		{
			perror("recv");
			exit(-1);
		}
		printf("Received msg: %s", buf);

		//send msg
		strcpy(buf, "Welcome to server");
		send(connfd, buf, BUFFER_SIZE, 0);
			close(connfd);
	}

	close(listenfd);
	exit(0);
}
