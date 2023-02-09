#include<stdio.h>
#include<errno.h>
#include<string.h>

#define N 128

int main(int argc,char *argv[])
{
	int n;
	char buf[N];
	FILE *fm, *ft;

	if(argc != 3)
	{
		printf("Usage : %s <src-file> <dst-file>",argv[0]);
		return -1;
	}

	if((fm = fopen(argv[1],"r")) == NULL)
	{
		fprintf(stderr,"fail to fopen %s : %s",argv[1],strerror(errno));
		return -1;
	}

	if((ft = fopen(argv[2],"w")) == NULL)
	{
		fprintf(stderr,"fail to fopen %s : %s",argv[2],strerror(errno));
		return -1;
	}
	
	memset(buf,0,sizeof(buf));
	while((n = fread(buf,1,N,fm)) > 0 )
	{
		fwrite(buf,1,N,ft);
	}

	fclose(fm);
	fclose(ft);
	return 0;
}
