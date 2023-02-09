#include <stdio.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <string.h>
#include <pthread.h>
#include "chat.h"

#define BUF_ZISE    1024

void* start_read(void *arg) // 读取信息的子线程
{
	int ret = -1;
	int clifd = *(int*)arg;
	char buf[BUF_ZISE] = {0};

	while(1)
	{
		memset(buf, 0, sizeof(buf));
		ret = read(clifd, buf, sizeof(buf));
		printf("say:%s\n",buf);
	}
}

void* start_write(void *arg) // 写回的子线程
{
#if 0
//	printf("arg:%d\n",*(int*)arg);

//	usleep(500);

	int clifd1 = *(int*)arg;

	printf("run_clifd:%d\n",clifd1);

	for(;;)
	{
		int flag = 0;
		for(int i=0; i<clifd_index; i++) // 因为读到quit的原因，clifd被置0
		{
			if(clifd1 == clifd[i])
			{
				break;
			}
			if(i == clifd_index-1)
			{
				int *die = &clifd1;
				flag = 1;
				pthread_exit(die); // 终止此写回的子线程
			}
		}
		if(flag == 1)
		{
			break;
		}

		if(strlen(str) == 0) // 空消息不写入
			continue;
		printf("before write\n");
		printf("str:%s\n",str);
		write(clifd1,str,strlen(str)+1);
		usleep(50000); // 最快的子线程等待其他子线程
		memset(str,0,1024); // 清空str
	}
#endif
}

int main()
{
	int ret = -1;
	int sockfd = -1;
	int clifd = -1;
	struct sockaddr_in addr = {0};
	struct sockaddr_in addrcli = {0};
	pthread_t pid1,pid2;

	printf("创建数据库...\n");
	ret = sql_init();
	if (ret < 0)
	{
		return -1;
	}

	printf("服务器创建socket...\n");
	sockfd = socket(AF_INET,SOCK_STREAM,0);
	if(0 > sockfd)
	{
		perror("socket");
		return -1;
	}

	printf("准备地址...\n");
	
	addr.sin_family = AF_INET;
	addr.sin_port = htons(SERVER_IP_PORT);
	addr.sin_addr.s_addr = inet_addr(SERVER_IP_ADDR);
	socklen_t len = sizeof(addr);

	printf("绑定socket与地址...\n");
	if(bind(sockfd,(struct sockaddr*)&addr,len))
	{
		perror("bind");
		return -1;
	}

	printf("设置监听...\n");
	if(listen(sockfd,5))
	{
		perror("listen");
		return -1;
	}

	printf("等待客户端连接...\n");
	while(1)
	{
		clifd = accept(sockfd,(struct sockaddr*)&addrcli,&len);

		// 创建子线程
		pthread_create(&pid1,NULL,start_read,&clifd);
		pthread_create(&pid2,NULL,start_write,&clifd);
	}
	return 0;
}
