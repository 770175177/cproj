#include "share.h"

int main(int argc, const char *argv[])
{
	int select = -1;
	char addr[32];
	short port = SERV_PORT;

	strncpy(addr, SERV_IP_ADDR, sizeof(SERV_IP_ADDR));
	
	if((select = previous_work(argc, argv)) < 0)
	{
		printf("Usage: %s %s [address port]", argv[0], argv[1]);
		exit(-1);
	}
	if(argc > 2)
	{
		strncpy(addr, argv[2], sizeof(argv[2]));
	}
	if(argc > 3)
	{
		port = atoi(argv[3]);
	}

	if(!select)
	{
		server(addr, port);
	}
	else
	{
		client(addr, port);
	}

	return 0;	
}

int previous_work(int argc, const char **argv)
{
	if(argc < 2)
	{
		return -1;
	}
	if(strncmp(argv[1], "server", 6) == 0)
	{
		return 0;
	}
	else if(strncmp(argv[1], "client", 6) == 0)
	{
		return 1;
	}
	else
	{
		return -1;
	}
}

void server(char *addr, short port)
{
	int fd = -1;
	int client_fd = -1;
	int b_reuse = 1;
	void *ret;
	struct sockaddr_in sin;
	struct sockaddr_in cin;
	socklen_t addrlen;
	pthread_t tid;
	MY_PARAM param;

	if((fd = socket(AF_INET, SOCK_STREAM, 0)) == -1)
	{
		perror("socket");
		exit(-1);
	}
	setsockopt(fd, SOL_SOCKET, SO_REUSEADDR, &b_reuse, sizeof(int));
	
	bzero(&sin, sizeof(sin));
	sin.sin_family = AF_INET;
	sin.sin_port = htons(port);
	sin.sin_addr.s_addr = inet_addr(addr);

	if(bind(fd, (struct sockaddr *)&sin, sizeof(sin)) == -1)
	{
		perror("bind");
		exit(-1);
	}
	if(listen(fd, BACKLOG) < 0)
	{
		perror("listen");
		exit(-1);
	}
	printf("server starting...OK\n");	

	addrlen = sizeof(cin);
	while(1)
	{
		char ipv4_addr[16];	
		if((client_fd = accept(fd, (struct sockaddr *)&cin, &addrlen)) < 0)
		{
			perror("accept");
			exit(-1);
		}
	
		if(!inet_ntop(AF_INET, (void*)&cin.sin_addr, ipv4_addr, sizeof(cin)))
		{
			perror("inet_ntop");
			exit(-1);
		}
		printf("client(%s: %d) is connected!\n", ipv4_addr, ntohs(cin.sin_port));
		
		param.fd = client_fd;
		if(pipe(param.pipe_fd) < 0)
		{
			printf("pipe error\n");
		}
		pthread_create(&tid, NULL, (void *)read_handler, (void *)&param);
		pthread_create(&tid, NULL, (void *)write_handler, (void *)&param);
		pthread_join(tid, &ret);
	}
	close(fd);
}

void client(char *addr, short port)
{
	int fd = -1;
	void *ret;
	struct sockaddr_in sin;
	pthread_t tid;
	//int pipe[fd];
	MY_PARAM param;
	
	if(pipe(param.pipe_fd) < 0)
	{
		printf("pipe error\n");
	}
	if((fd = socket(AF_INET, SOCK_STREAM, 0)) == -1)
	{
		perror("socket");
		exit(-1);
	}
	bzero(&sin, sizeof(sin));
	sin.sin_family = AF_INET;
	sin.sin_port = htons(port);
	if(inet_pton(AF_INET, addr, (void *)&sin.sin_addr.s_addr) != 1)
	{
		perror("inet_pton");
		exit(-1);
	}
	if(connect(fd, (struct sockaddr *)&sin, sizeof(sin)) < 0)
	{
		perror("connect");
		exit(-1);
	}
	printf("client connected...OK!\n");
	
	param.fd = fd;
	pthread_create(&tid, NULL, (void *)write_handler, (void *)&param);
	pthread_create(&tid, NULL, (void *)read_handler, (void *)&param);
	pthread_join(tid, &ret);

	close(fd);
}

void get_file_name(char dest[], const char src[], int flag)
{
	int i, j = 0;
	if(!flag)
	{
		i = CMD_LEN;
		while(++i == ' ');

		while(i < strlen(src)-1)
		{
			dest[j++] = src[i++];
		}
	}
	else
	{
		strncpy(dest, CMD, CMD_LEN);
		j = CMD_LEN;
		for(i = 0; i < strlen(src); i++)
		{
			dest[j++] = src[i];

			if(src[i] == '/')
			{
				j = 0;
			}
		}
	}
	dest[j] = '\0';
}

void write_handler(void *arg)
{
	MY_PARAM *param = (MY_PARAM *)arg;
	int client_fd = param->fd;
	int file_fd;
	int ret = -1;
	char buf[BUFSIZE];
	char path[128];
	char ack[16];
	struct stat statbuf;
	MY_FILE myfile;

	while(1)
	{
		printf("Send >> ");
start:		bzero(buf, BUFSIZE);
		if(fgets(buf, BUFSIZE, stdin) == NULL)
		{
			continue;
		}
		if(strncmp(buf, CMD, CMD_LEN) == 0)
		{
			bzero(&myfile, sizeof(myfile));
			get_file_name(path, buf, 0);
		
			if((file_fd = open(path, O_RDONLY)) < 0)
			{
				printf("no such path, please retry\n");
				continue;
			}
			stat(path, &statbuf);
			myfile.file_size = statbuf.st_size;
			if(myfile.file_size > FILE_MAXSIZE)
			{
				printf("file size is %lld(>10MB), please retry\n", myfile.file_size/1024/1024);
				continue;
			}
			//printf("open file: %s ok\n", path);

			bzero(buf, sizeof(buf));
			strncpy(buf, CMD, CMD_LEN);
			get_file_name(myfile.file_name, path, 1);
			memcpy(buf+CMD_LEN, &myfile, sizeof(myfile));
			//printf("file name: %s, file size: %lld\n", myfile.file_name, myfile.file_size);
			//printf("buf:%s\n", buf);
			write(client_fd, buf, sizeof(buf));

			bzero(buf, sizeof(buf));
			read(param->pipe_fd[0], buf, BUFSIZE);
			
			//printf("\nrecv stat=%s\n", buf);
			if(strncmp(buf, IS_FILE_EXIST, FILE_EXIST_len) == 0)
			{
				printf("file %s exist, do you want to overwrite?, y/n\n", myfile.file_name);
				fflush(stdout);
				scanf("%s", ack);
				if(strncasecmp(ack, "n", 1) == 0)
				{
					bzero(buf, sizeof(buf));
					strncpy(buf, NOT_OVER_WRITE, NOT_OVER_WRITE_LEN);
					write(client_fd, buf, sizeof(buf));
					goto start;
				}
				bzero(buf, sizeof(buf));
				strncpy(buf, OVER_WRITE, OVER_WRITE_LEN);
			}
			else
			{
				bzero(buf, sizeof(buf));
				strncpy(buf, OVER_WRITE, OVER_WRITE_LEN);
			}
			write(client_fd, buf, sizeof(buf));

			bzero(buf, sizeof(buf));
			while(ret = read(file_fd, buf, BUFSIZE))
			{
				if(ret < 0)
				{
					perror("read");
					continue;
				}
				write(client_fd, buf, ret);
				bzero(buf, sizeof(buf));
				if(ret < BUFSIZE)
				{
					break;
				}
			}
			printf("transfer complite\n");
			fflush(stdout);
			close(file_fd);
		}
		else
		{
			write(client_fd, buf, strlen(buf));
			if(strncasecmp(buf, QUIT_STR, QUIT_LEN) == 0)
			{
				printf("client(fd=%d) is exiting!\n", client_fd);
				break;
			}
		}
	}
}

void read_handler(void *arg)
{
	MY_PARAM *param = (MY_PARAM *)arg;
	int client_fd = param->fd;
	int ret = -1;
	int fd_file;
	char buf[BUFSIZE];
	int time, n;
	char file_path[128];
	MY_FILE *myfile;
	long long file_size;
	long long count = 0;

	while(1)
	{
r_start:	bzero(buf, sizeof(buf));
		do{
			ret = read(client_fd, buf, BUFSIZE);
		}while(ret < 0 && EINTR == errno);
		if(ret < 0)
		{
			perror("read");
			exit(-1);
		}
		if(strncmp(buf, IS_FILE_EXIST, FILE_EXIST_len) == 0)
		{
			write(param->pipe_fd[1], IS_FILE_EXIST, FILE_EXIST_len);
			goto r_start;
		}
		if(strncmp(buf, FILE_NOT_EXIST, NOT_EXIST_len) == 0)
		{
			write(param->pipe_fd[1], FILE_NOT_EXIST, NOT_EXIST_len);
			goto r_start;
		}
		if(strncmp(buf, CMD, CMD_LEN) == 0)
		{
			myfile = (MY_FILE *)(buf+CMD_LEN);
			bzero(file_path, sizeof(file_path));
			strncpy(file_path, myfile->file_name, strlen(myfile->file_name));
			file_size = myfile->file_size;
			//printf("file:%s, size=%lld\n", file_path, file_size);

			bzero(buf, sizeof(buf));
			//strncpy(buf, ACK, ACK_LEN);
			if(access(file_path, F_OK) == 0)
			{
				//*(buf+ACK_LEN) = 1;
				strncpy(buf, IS_FILE_EXIST, FILE_EXIST_len);
			}
			else
			{
				//*(buf+ACK_LEN) = 0;
				strncpy(buf, FILE_NOT_EXIST, NOT_EXIST_len);
			}
			//printf("send file status ack: %s\n", buf);
			write(client_fd, buf, sizeof(buf));

			bzero(buf, sizeof(buf));
			if((ret = read(client_fd, buf, BUFSIZE)) < 0)
			{
				perror("read");
				continue;
			}
			//printf("%s\n", buf);
			if(strncmp(buf, NOT_OVER_WRITE, NOT_OVER_WRITE_LEN) == 0)
			{
				printf("file exist, do nothing");
				goto r_start;
			}

			bzero(buf, sizeof(buf));
			if((fd_file = open(file_path, O_WRONLY|O_CREAT|O_TRUNC, 0777)) < 0)
			{
				perror("open");
				continue;
			}
			printf("\nopen file ok...\n");
			while(count <= file_size)
			{
				if((ret = read(client_fd, buf, BUFSIZE)) < 0)
				{
					perror("read");
					continue;
				}
				write(fd_file, buf, ret);
				count += ret;
				bzero(buf, sizeof(buf));

				if(ret < BUFSIZE || count == file_size)
				{
						break;
				}
			}
			printf("file size = %lld, recieve complete\nSend >> ", count);
			fflush(stdout);
			close(fd_file);
		}
		else
		{
			if(ret == 0)
			{
				break;
			}
			n++;
			printf("\nRecv[%d] << %sSend >> ",client_fd, buf);
			fflush(stdout);
			if(strncasecmp(buf, QUIT_STR, QUIT_LEN) == 0)
			{
				printf("client(fd=%d) is exiting!\n", client_fd);
				break;
			}
		}
	}
	close(client_fd);
}
