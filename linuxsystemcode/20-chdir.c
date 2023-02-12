#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

#define LENGTH 255

int main(int argc, const char *argv[])
{
	int ret;
	char pwd[LENGTH];
	
	if(argc < 2)
	{
		printf("please input file path\n");
		return -1;
	}
	if(!getcwd(pwd, LENGTH))
	{
		perror("getcwd");
		return -1;
	}
	printf("current path is %s\n", pwd);

	if(chdir(argv[1]) < 0)
	{
		perror("chdir");
		return -1;
	}
	if(!getcwd(pwd, LENGTH))
	{
		perror("getcwd");
		return -1;
	}
	printf("current path is %s\n", pwd);
	
	return 0;
}
