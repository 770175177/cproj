#include <stdio.h>
#include <sys/time.h>

void function()
{
	unsigned int i, j;
	double y;

	for(i=0; i<1000; i++)
		for(j=0; j<1000; j++)
			y = i / (j + 1);
}

int main()
{
	struct timeval tpstart, tpend;
	float timeuse;

	gettimeofday(&tpstart, NULL);
	function();

	gettimeofday(&tpend, NULL);
	timeuse = 1000000 * (tpend.tv_sec - tpstart.tv_sec) + (tpend.tv_usec - tpstart.tv_usec);

	timeuse /= 1000000;
	printf("used time: %f\n", timeuse);

	return 0;
}
