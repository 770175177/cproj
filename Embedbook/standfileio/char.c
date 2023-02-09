#include<stdio.h>

int main()
{
	int c;
	fputc(fgetc(stdin),stdout);
}
