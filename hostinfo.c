#include <stdio.h>
#include <arpa/inet.h>
#include <stdlib.h>
#include <netdb.h>
#include <string.h>

int main()
{
	struct hostent *hostp;
	char cmdline[100];
	char **pp;
	struct in_addr addr;
	
	fgets(cmdline, 100, stdin);
	
	cmdline[strlen(cmdline)-1] = '\0';
	
	if((hostp = gethostbyname(cmdline)) == NULL){
		printf("error : %d \n",h_errno);
		exit(0);
	}
	
	printf("name : %s \n",hostp->h_name);
	
	for(pp = hostp->h_aliases; *pp != NULL ; pp++){
		printf("ali: %s \n",*pp);
	}
	
	for(pp = hostp->h_addr_list; *pp != NULL;pp++){
		addr.s_addr = ((struct in_addr *)*pp)->s_addr;
		printf("addr: %s \n",inet_ntoa(addr));
		printf("addr: %d \n",addr.s_addr);
	}
	exit(0);
	
}