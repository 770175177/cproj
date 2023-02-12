#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>

#define BUF_LEN 255
#define __USE_GNU

int main(int argc, const char *argv[])
{
	char pwd[BUF_LEN];
	char *wd;

	if(getcwd(pwd, BUF_LEN) == NULL)
	{
		printf("getcwd wrror!\n");
		return -1;
	}
	printf("current direct is: %s\n", pwd);

	if((wd = getwd(pwd)) == NULL)
	{
		printf("getwd wrror!\n");
		return -1;	
	}
	printf("current direct is: %s\n", wd);

	if((wd = get_current_dir_name()) == NULL)
	{
		printf("get_current_dir_name error!\n");
		return -1;
	}
	printf("current direct is: %s\n", wd);

	return 0;
}
