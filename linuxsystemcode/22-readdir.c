#include <stdio.h>
#include <sys/types.h>
#include <dirent.h>
#include <string.h>

int main(int argc, const char *argv[])
{
	DIR *dir;
	struct dirent *catlog;

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
	printf("opendir %s success\n", argv[1]);

	while((catlog = readdir(dir)) != NULL)
	{
		if(!strncmp(catlog->d_name, ".", 1) || !(strncmp(catlog->d_name, "..", 2)))
			continue;
		printf("%s\t", catlog->d_name);
		printf("\n");
	}
//	printf("inode is %d, offset is %d, len is %d, type is %d, name is %s\n", catlog->d_ino, catlog->d_off, catlog->d_reclen, catlog->d_type, catlog->d_name);

	closedir(dir);

	return 0;
}
