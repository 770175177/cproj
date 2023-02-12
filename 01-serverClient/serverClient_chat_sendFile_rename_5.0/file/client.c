#include "net.h"

void usage(const char *s);
void input_handler(void *args);
void output_handler(void *args);
void get_file_name(char dest[], const char src[], int flag);

int main(int argc, const char *argv[])
{
	int fd = -1;
	void *ret;
	short port;
	struct sockaddr_in sin;
	pthread_t tid_in, tid_out;
	
	if(argc != 3)
	{
		usage(argv[0]);
		exit(-1);
	}
	port = atoi(argv[2]);
	if(port < 5000)
	{
		usage(argv[0]);
		exit(-1);
	}
	if((fd = socket(AF_INET, SOCK_STREAM, 0)) == -1)
	{
		perror("socket");
		exit(-1);
	}
	bzero(&sin, sizeof(sin));
	sin.sin_family = AF_INET;
	sin.sin_port = htons(port);
	//sin.sin_addr.s_addr = inet_addr(SERV_IP_ADDR);
	if(inet_pton(AF_INET, argv[1], (void *)&sin.sin_addr.s_addr) != 1)
	{
		perror("inet_pton");
		exit(-1);
	}
	if(connect(fd, (struct sockaddr *)&sin, sizeof(sin)) < 0)
	{
		perror("connect");
		exit(-1);
	}
	printf("client starting...OK!\n");

	pthread_create(&tid_in, NULL, (void *)input_handler, (void *)&fd);
	pthread_create(&tid_out, NULL, (void *)output_handler, (void *)&fd);
	//pthread_join(tid, &ret);
	while(1);

	close(fd);

	return 0;
}

void usage(const char *s)
{
	printf("Usage: %s serv_ip serv_port\n", s);
	printf("serv_ip: server ip address\n");
	printf("serv_port: server port(>5000)\n\n");
}

void output_handler(void *args)
{
	int fd = *(int *)args;
	int file_fd;
	int ret = -1;	
	char buf[BUFSIZE];
	char path[128];

	while(1)
	{
kk:		bzero(buf, sizeof(buf));
		if((ret = read(fd, buf, BUFSIZE-1)) < 0)
		{
			printf("read");
			exit(-1);;
		}
		if(strncmp(buf, "pic", 3) == 0)
		{
			bzero(path, sizeof(path));
			strncpy(path, buf+4, strlen(buf)-4);
			if((file_fd = open(path, O_WRONLY|O_CREAT|O_TRUNC, 0777)) < 0)
			{
				perror("open");
				continue;
			}
			while(ret = read(fd, buf, BUFSIZE))
			{
				if(ret < 0)
				{
					perror("read");
					continue;
				}
				if(ret < BUFSIZE)
				{
					write(file_fd, buf, ret-1);
					printf("write file ok\n");
					printf("Send >> ");
					fflush(stdout);
					close(file_fd);
					bzero(buf, sizeof(buf));
					goto kk;
				}
				write(file_fd, buf, ret);
				bzero(buf, sizeof(buf));
			}
		}
		else
		{
			if(ret == 0)
			{
				break;
			}
			printf("\nRecv << %sSend >> ", buf);
			fflush(stdout);
			if(strncasecmp(buf, QUIT_STR, 4) == 0)
			{
				printf("client is exiting!\n");
				break;
			}
		}
	}
}

void input_handler(void *args)
{
	int fd = *(int  *)args;	
	int fd_file;
	char buf[BUFSIZE];
	char pic_path[128];
	int ret = -1;
	int count, index;
	
	//printf("Please input your name\ninput >> ");
	//bzero(buf, sizeof(buf));
	//while(fgtes(buf, 16, stdin) == NULL);	
	printf("input >> ");

	while(1)
	{
		count = 0;
		index = 0;
		bzero(buf, sizeof(buf));
		if(fgets(buf, BUFSIZE-1, stdin) == NULL)
		{
			continue;
		}
		if(strncasecmp(buf, "pic", 3) == 0)
		{
			get_file_name(pic_path, buf, 0);
			printf("%s\n", pic_path);
			if((fd_file = open(pic_path, O_RDONLY)) < 0)
			{
				printf("no such a path, please retry\n");
				printf("input >> ");
				continue;
			}
			printf("open file %s ok\n", pic_path);
			bzero(buf, sizeof(buf));
			get_file_name(buf, pic_path, 1);
			printf("%s\n", buf);
			write(fd, buf, strlen(buf));	
			bzero(buf, sizeof(buf));
			while(ret = read(fd_file, buf, BUFSIZE))
			{
				if(ret < 0)
				{
					perror("read");
					continue;
				}
				//printf("%s\n", buf);
				count += ret;
				printf("read %d, ret = %d\n", index++, ret);
				write(fd, buf, ret);
				bzero(buf, sizeof(buf));
				if(ret < BUFSIZE)
				{
					break;
				}
			}
			printf("tranfer complet\n");	
			printf("count is %d\n", count);
			printf("Send >> ");
			close(fd_file);
		}
		else
		{
			write(fd, buf, strlen(buf));
			printf("Send >> ");
			if(strncasecmp(buf, QUIT_STR, 4) == 0)
			{
				printf("client is exiting!\n");
				break;
			}
		}
	}
}
void get_file_name(char dest[], const char src[], int flag)
{
	int i, j = 0;
	if(!flag)
	{
		for(i = 0; i < strlen(src)-5; i++)
		{
			dest[j] = src[i+4];
			j++;
		}
	}
	else
	{
		strncpy(dest, "pic ", 4);
		j = 4;
		for(i = 0; i < strlen(src); i++)
		{
			dest[j] = src[i];
			j++;
			if(src[i] == '/')
			{
				//i++;
				j = 4;
			}
		}
	}
	dest[j] = '\0';
	j = 0;
}
