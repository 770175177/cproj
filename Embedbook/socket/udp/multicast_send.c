#include<stdio.h>
#include<sys/types.h>
#include<sys/socket.h>
#include<stdlib.h>
#include<string.h>
#include<unistd.h>
#include<netinet/in.h>
#include<arpa/inet.h>

#define N 64
typedef struct sockaddr SA;

int main(int argc, char * argv[])
{
	int sockfd;
	struct sockaddr_in destaddr;
	char buf[N] = "This id a multicast pakage\n";
	int on = 1;

	if(argc < 3)
	{
		printf("Usage : %s <ip> <port>\n",argv[0]);
		exit(-1);
	}

	//create socket
	if((sockfd = socket(PF_INET, SOCK_DGRAM, 0))  == -1)
	{
		perror("socket");
		exit(-1);
	}

	//set param of sockaddr_in
	bzero(&destaddr, sizeof(destaddr));
	destaddr.sin_family = PF_INET;
	destaddr.sin_port = htons(atoi(argv[2]));
	destaddr.sin_addr.s_addr = inet_addr(argv[1]);

	while(1)
	{
		//send to server
		sendto(sockfd, buf, N, 0, (SA *)&destaddr, sizeof(destaddr));
		sleep(1);
	}
	
	return (0);
}
