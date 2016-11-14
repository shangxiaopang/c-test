#include <stdio.h>
#include <arpa/inet.h>
#include <stdlib.h>

int main ()
{
	char cmdline[1024];
	struct in_addr inp;
	
	fgets(cmdline, 1024, stdin);
	
	if (feof(stdin)){
		exit(0);
	}
	if(inet_aton(cmdline,&inp) == 0){
		printf("error \n");
		exit(0);
	}
	
	printf("ip to a: %02X \n",inp.s_addr);
}
	