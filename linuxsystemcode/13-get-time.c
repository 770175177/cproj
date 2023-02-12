#include <stdio.h>
#include <time.h>

int main()
{
	time_t t;

	time(&t);
	printf("UTC time is: 0x%08x\n", t);

	t = time(NULL);
	printf("UTC time is: 0x%08x\n", t);

	return 0;
}
