#ifndef __SHARE_H__
#define __SHARE_H__

#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <errno.h>
#include <strings.h>
#include <string.h>
#include <netinet/in.h>
#include <netinet/ip.h>
#include <pthread.h>

#define SERV_PORT 5000
#define SERV_IP_ADDR "127.0.0.1"
#define BACKLOG 5
#define BUFSIZE 1024
#define QUIT_STR "quit"
#define QUIT_LEN (sizeof(QUIT_STR)-1)
#define CMD "!file"
#define CMD_LEN (sizeof(CMD)-1)
#define IS_FILE_EXIST 	"ext"
#define FILE_EXIST_len (sizeof(IS_FILE_EXIST)-1)
#define FILE_NOT_EXIST 	"nox"
#define NOT_EXIST_len (sizeof(FILE_NOT_EXIST)-1)
#define OVER_WRITE	"overw"
#define OVER_WRITE_LEN (sizeof(OVER_WRITE))
#define NOT_OVER_WRITE	"not_overw"
#define NOT_OVER_WRITE_LEN (sizeof(NOT_OVER_WRITE))
#define FILE_MAXSIZE (10*1024*1024LL)

typedef struct _myfile{
	char file_name[32];
	long long file_size;
}MY_FILE;

typedef struct _param{
	int fd;
	int pipe_fd[2];
}MY_PARAM;

void read_handler(void *arg);
void write_handler(void *arg);
void get_file_name(char dest[], const char src[], int flag);
int previous_work(int argc, const char **argv);
void server(char *addr, short port);
void client(char *addr, short port);

#endif
