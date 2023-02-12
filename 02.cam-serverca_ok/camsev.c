
#include<stdio.h>
#include<signal.h>
#include<stdlib.h>
#include<unistd.h>
#include<string.h>
#include <fcntl.h>

#include<sys/types.h>
#include<sys/wait.h>
#include<sys/socket.h>

#include<netinet/in.h>
#include<netinet/ip.h>

#include<arpa/inet.h>

#include <pthread.h>
#include <semaphore.h>

#define devpath "/dev/video0"

sem_t sem0,sem3;

//static int pix_size;
static int pic_size=0;

static unsigned int num_connect=0;
static unsigned int num_send=0;

char pic1[320*480];

void *pthread_func(void *arg)
{
	unsigned int width = 640;
	unsigned int height = 480;
	unsigned int size;
	unsigned int ismjpeg;
	
	int pix_size;
	
	unsigned int index;
	int i;
	
	int fd;
	void *buf;
	
	if( -1 == (fd = camera_init(devpath,&width,&height,&size,&ismjpeg)) )
	{
		printf("camera_init failed!\n");
		pthread_exit(NULL);
	}
	if( -1 == camera_start(fd) )
	{
		printf("camera_start failed!\n");
		camera_stop(fd);
		camera_exit(fd);
		pthread_exit(NULL);
	}
	while(1)
	{
		memset(&index,0,sizeof(index));
		if( -1 == camera_dqbuf(fd,&buf,&pix_size,&index) )
		{
			printf("camera_dqbuf failed!\n");
			camera_stop(fd);
			camera_exit(fd);
			break;
		}
		//printf("line = %d,ret = %d\n",__LINE__);
		if(num_connect > 0 && num_send == num_connect)   //当前有连接，且发送数与连接数相等，
		{												//			即所有的用户，数据都发送完毕
			memset(pic1,0,sizeof(pic1));				//        	则捕捉下一帧数据，发送
			memcpy(pic1,buf,pix_size);
			pic_size=pix_size;
			for(i=0;i<num_connect;i++)
				num_send--;
			for(i=0;i<num_connect;i++)
				sem_post(&sem0);
			printf("line = %d\n",__LINE__);
		}
		//printf("line = %d\n",__LINE__);

		if( -1 == camera_eqbuf(fd,index) )
		{
			printf("camera_eqbuf failed!\n");
			camera_stop(fd);
			camera_exit(fd);
			break;
		}
				
	}
}
void *write_func(void *arg)
{
	int ret;
	char buf_size[10];
	char pthread_buf[320*480];
	int sockfd = *((int*)arg);
	while(1)
	{
		//printf("line = %d\n",__LINE__);
		sem_wait(&sem0);
		sprintf(buf_size,"%d",pic_size);
		ret = write(sockfd,buf_size,sizeof(buf_size));
		if( 0 >= ret ) 
		{
			perror("write size");			
			break;
		}
		printf("size= %d\n",ret);
		memset(pthread_buf,0,sizeof(pthread_buf));
		memcpy(pthread_buf,pic1,sizeof(pic1));
		ret = write(sockfd,pthread_buf,pic_size);
		//ret = write(sockfd,pic1,pix_size);
		if( 0 >= ret )
		{
			perror("write data");
			break;
		}
		printf("data= %d\n",ret);
		num_send++;
	}
	close(sockfd);
	free(arg);
	num_connect--;
	printf("sockfd close\n");
}

int main()
{
	struct sigaction sa;
	sa.sa_handler = SIG_IGN;
	sigaction( SIGPIPE, &sa, 0 );   //设置SIGPIPE信号处理方式为忽略
	
	if(sem_init(&sem0,0,0)<0)
	{
		perror("sem0 init");
		return -1;
	}
	int listenfd;
	if( -1 == (listenfd = socket(AF_INET,SOCK_STREAM,0)) )
	{
		perror("socket");
		exit(-1);
	}
	struct sockaddr_in sevaddr;
	memset(&sevaddr,0,sizeof(sevaddr));
	sevaddr.sin_family = AF_INET;
	sevaddr.sin_port = htons(8888);
	sevaddr.sin_addr.s_addr = htonl(INADDR_ANY);

	if( -1 == bind(listenfd,(const struct sockaddr *)&sevaddr,sizeof(sevaddr)) )
	{
		perror("bind");
		exit(-1);
	}

	if( -1 == listen(listenfd,10) )
	{
		perror("listen");
		exit(-1);
	}
	int sockfd;
	pid_t pid;
	/*
	unsigned int width = 640;
	unsigned int height = 480;
	unsigned int size;
	unsigned int ismjpeg;

	
	if( -1 == (fd = camera_init(devpath,&width,&height,&size,&ismjpeg)) )
	{
		printf("camera_init failed!\n");
		return -1;
	}
	
	if( -1 == camera_start(fd) )
	{
		printf("camera_start failed!\n");
		camera_stop(fd);
		camera_exit(fd);
		close(sockfd);
		return -1;
	}
	*/
	struct sockaddr cltaddr;
	socklen_t addrlen = sizeof(cltaddr);
	pthread_t pythread;
	if(pthread_create(&pythread,NULL,pthread_func,NULL) < 0)
	{
		perror("pthread create");
		return -2;
	}
	int *p;
	pthread_t pthread;
	while(1)
	{
		memset(&cltaddr,0,sizeof(cltaddr));
		if( -1 == (sockfd = accept(listenfd,&cltaddr,&addrlen)))
		{
			perror("sockfd");
			continue;
		}
		//printf("width = %d\nheight = %d\n",width,height);
		p = (int *)malloc(sizeof(int));		//开辟空间装套接字，传递给线程
		*p = sockfd;
		num_connect++;	//新的连接，记录连接数
		num_send++;		//记录发送完成数
		if(pthread_create(&pthread,NULL,write_func,p) < 0)
		{
			perror("pthread create");
			free(p);
			num_connect--;
			num_send--;
			continue;
		}
		printf("new connection....\n");
		pthread_detach(pthread);
	}

	close(listenfd);
	printf("line = %d\n",__LINE__);
	return 0;
}

