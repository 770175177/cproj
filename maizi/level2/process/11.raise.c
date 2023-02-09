#include<stdio.h>
#include<sys/types.h>
#include<stdlib.h>
#include<signal.h>
#include<fcntl.h>

int main(int argc, char *argv[])
{
	printf("raise before\n");
	raise(9);	//_exit	  exit
	printf("raise after\n");

	return 0;
}
