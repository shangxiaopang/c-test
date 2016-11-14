#include <sys/types.h>
#include <sys/socket.h>
#include <stdio.h>
#include <arpa/inet.h>
#include <string.h>
#include <netinet/in.h>
#include <errno.h>
#include <unistd.h>
#include "csapp.h"

typedef struct sockaddr SA;

int open_client(char *host,int port)
{
	struct sockaddr_in service_addr;
	struct hostent *hostp;
	struct in_addr addr;
	
	if(inet_aton(host,&addr) != 1){
		char * mesg = strerror(errno);
		printf("Mesg:%s\n",mesg);
		return -1;
	}
	
	int client_fd = socket(AF_INET,SOCK_STREAM,0);

	//初始化
	bzero((char *) &service_addr,sizeof(service_addr));
	
	service_addr.sin_family = AF_INET;
	service_addr.sin_addr = addr;
	service_addr.sin_port = htons((unsigned short)port);
	
	if(connect(client_fd,(SA *) &service_addr,sizeof(service_addr)) < 0){
		char * mesg = strerror(errno);
		printf("Mesg:%s\n",mesg);
		return -1;
	}
	return client_fd;
}

int main()
{
	int client_fd,port = 8898;
	char buf[1024];
	rio_t rio;
	// int i;
// 	for(i = 0; i < 2;i++){
// 		if(fork() == 0){
// 			client_fd = open_client("127.0.0.1",port);
// 			printf("client: %d \n",client_fd);
//
// 			while(1){
//
// 			}
//
// 		}
//
// 	}
	
	client_fd = open_client("127.0.0.1",port);
	
	Rio_readinitb(&rio,client_fd);
	
	sprintf(buf,"GET");
	
	Rio_writen(client_fd,buf,strlen(buf));
	
	Rio_readlineb(&rio,buf,1024);
	Fputs(buf,stdout);
	
	while(1)
		;

	close(client_fd);
	exit(0);
}

