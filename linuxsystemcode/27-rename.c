#include <stdio.h>

int main(int argc, const char *argv[])
{
	if(argc < 3)
	{
		printf("please input file path\n");
		return -1;
	}
	if((rename(argv[1], argv[2])) < 0)
	{
		perror("rename");
		return -1;
	}
	printf("remove %s to %s success!\n", argv[1], argv[2]);

	return 0;
}
