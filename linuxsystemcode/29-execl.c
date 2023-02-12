#include <stdio.h>
#include <unistd.h>
#include <errno.h>

int main()
{
	if(execl("./01-helloworld", "hello", "world", NULL) < 0)
	{
		perror("execl");
		return 0;
	}

	printf("not execute\n");

	return 0;
}
