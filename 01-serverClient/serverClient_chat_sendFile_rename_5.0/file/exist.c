#include <stdio.h>
#include <unistd.h>

int main()
{
	if(access("a.txt", F_OK) == 0)
	{
		printf("exist\n");
	}
	else
	{
		printf("not exist\n");
	}

	return 0;
}
