#include <stdio.h>
#include <unistd.h>

int main(int argc, const char *argv[])
{
	int ret;

	if(argc < 3)
	{
		printf("please inout file path\n");
		return -1;
	}
	if((ret = link(argv[1], argv[2])) < 0)
	{
		printf("link failed\n");
		return -1;
	}
	printf("link %s to %s success!\n", argv[1], argv[2]);

	return 0;
}
