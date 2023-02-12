#include <stdio.h>
#include <unistd.h>

int main(int argc, const char *argv[])
{
	int ret;

	if(argc < 2)
	{
		printf("please inout file path\n");
		return -1;
	}
	if((ret = unlink(argv[1])) < 0)
	{
		printf("unlink failed\n");
		return -1;
	}
	printf("unlink %s is success!\n", argv[1]);

	return 0;
}
