#include <stdio.h>

#define MAXARGS   128

int main(int argc,char **argv, char **envp)
{
	int i = 0;
	for(i = 0;i < sizeof(argv);i++){
		printf("%s \r\n",argv[i]);
	}
	int x = 0;
	for(x = 0;x < sizeof(envp);x++){
		printf("%s \r\n",envp[x]);
	}
}



