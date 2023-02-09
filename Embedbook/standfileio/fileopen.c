#include<stdio.h>

int main()
{
	FILE *f = fopen("io.txt","a");
	fwrite("hello\n",100,100,f);
	fclose(f);
}
