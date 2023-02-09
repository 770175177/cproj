#include<stdio.h>
#include<string.h>
#include<strings.h>
#include<arpa/inet.h>
#include<sys/types.h>
#include<sys/socket.h>
#include<sys/stat.h>
#include<time.h>

#define MAX_LISTEN 10

int main(int argc, char *argv[])
{
	int sockfd, listenfd, opt=1;
	struct sockaddr_in server, client;
	char buf[100], read_buf[100];
	int read_len = 0;
	socklen_t len;
	time_t timep;

	listenfd = socket(AF_INET, SOCK_STREAM, 0);
	if(listenfd < 0)
	{
		perror("create socket failed\n");
		return -1;
	}

	if(setsockopt(listenfd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt) ) < 0)
	{
		perror("set socket reuse addr failed");
		return -1;
	}

	bzero(&server, sizeof(server));
	server.sin_family = AF_INET;
	server.sin_addr.s_addr = htonl(INADDR_ANY);
	server.sin_port = htons(1236);

	len = sizeof(struct sockaddr);
	if(bind(listenfd, (struct sockaddr*)&server, len) < 0)
	{
		perror("bind error");
		return -1;
	}

	listen(listenfd, MAX_LISTEN);
	while(1)
	{
		sockfd = accept(listenfd, (struct sockaddr *)&client, &len);
		if(sockfd < 0)
		{
			perror("accept error");
			return -1;
		}
		
//		sleep(10);		
		timep = time(NULL);
		snprintf(buf, sizeof(buf), "%s", ctime(&timep));
		write(sockfd, buf, strlen(buf));
		printf("Bytes: %d\n", strlen(buf));
		printf("sockfd=%d\n", sockfd);
		printf("IP:%x, Port:%d\n",ntohl(client.sin_addr.s_addr), ntohs(client.sin_port));
		//sleep(10);
		read_len = read(sockfd, buf, 100);
		if(read_len < 0)
		{
			printf("read error\n");
			return -1;
		}
		if(read_len == 0)
		{
			printf("client connection closed\n");
		}

		close(sockfd);	
	}

	return 0;
}
