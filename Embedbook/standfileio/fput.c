#include<stdio.h>

int main()
{
	int c;
	while(1)
	{
		c = getc(stdin);
		if((c >= '0') && (c <= '9')) fputc(c,stdout);
		if(c == '\n') break;
	}

	return 0; 
}
