#include <stdio.h>
#include <unistd.h>

int main()
{
	int i = 10;
	while(i--)
	{
		sleep(2);
		printf("sleep 2s !\n");
		usleep(1000000);
		printf("usleep 1000000 !\n");
	}

	return 0;
}
