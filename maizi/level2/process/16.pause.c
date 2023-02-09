#include<stdio.h>
#include<sys/types.h>
#include<stdlib.h>
#include<signal.h>
#include<fcntl.h>

int main(int argc, char *argv[])
{
	int i=0;
	
	printf("pause before\n");
	pause();
	printf("pause after\n");
	while(1)
	{
		i++;
		sleep(1);
		printf("process things, i=%d\n", i);
	}

	return 0;
}
