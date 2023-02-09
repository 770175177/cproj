#include<unistd.h>
#include<stdio.h>
#include<stdlib.h>

int main()
{
	alarm(5);
	pause();
	printf("I have been waken up.\n");
}
