#include<stdio.h>
#include<string.h>
#include<strings.h>
#include<arpa/inet.h>
#include<sys/types.h>
#include<sys/socket.h>
#include<sys/stat.h>
#include<time.h>

int main()
{
	int sockfd;
	struct sockaddr_in server, client;
	char buf[200], read_buf[100];
	unsigned int len, length;
	time_t timep;
	int rv, opt;

	sockfd = socket(AF_INET, SOCK_DGRAM, 0);
	if(sockfd < 0)
	{
		perror("create socket failed");
		return -1;	
	}
	if(setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)) < 0)
         {
                 perror("set socket reuse addr failed");
                 return -1;
         }
	
	printf("create socket success\n");
	bzero(&server, sizeof(server));
	server.sin_family = AF_INET;
	server.sin_port = htons(1234);
	server.sin_addr.s_addr = htonl(INADDR_ANY);
	
	len = sizeof(struct sockaddr);
	if(bind(sockfd, (struct sockaddr *)&server, len) < 0)
	{
		perror("bind error");
		return -1;
	}
	printf("bind success\n");
	
	while(1)
	{
		bzero(&read_buf, 100);
		bzero(&client, len);
		rv = recvfrom(sockfd, read_buf, 100, 0, (struct sockaddr *)&client,&len);
		if(rv < 0)
		{
			printf("recvfrom error!\n");
			close(sockfd);
			return -1;
		}
		printf("len: %d\n", rv);
		printf("ip: %x, port: %d\n", ntohl(client.sin_addr.s_addr), ntohs(client.sin_port));
		printf("read_buf: %s", read_buf);

		bzero(&buf, 200);
		timep = time(NULL);
		snprintf(buf, sizeof(buf), "%s", ctime(&timep));
		
		sendto(sockfd, buf, strlen(buf), 0, (struct sockaddr *)&client, len);
	}
	return 0;
}
