#include <stdio.h>
#include <sys/types.h>
#include <dirent.h>

int main(int argc, const char *argv[])
{
	DIR *dir;

	if(argc < 2)
	{
		printf("please input file path\n");
		return -1;
	}
	if((dir = opendir(argv[1])) == NULL)
	{
		perror("opendir");
		return -1;
	}
	printf("printf %s success\n", argv[1]);

	closedir(dir);

	return 0;
}
