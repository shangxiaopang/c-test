#include <stdio.h>
int main()
{
	int a=1,*b;
	
	b = &a;
	
	printf("%d \n",b);
	printf("%d \n",*b);
	
	char c[10];
	
	char *p;
	
	c[0] = 'c';
	
	p = c;
	
	printf("%c \n",p[0]);
	
	char *d[10];
	char **e;
	char f = 'f';
	
	d[0] = &f;
	e = d;
	
	printf("%p %c \n",d[0],*d[0]);
	
	printf("%p %c \n",e[0],*e[0]);
	
	e[0] = "12312";
	printf("%p %c \n",e[0],*e[0]);
}