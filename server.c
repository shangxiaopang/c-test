#include <sys/types.h>
#include <sys/socket.h>
#include <stdio.h>
#include <arpa/inet.h>
#include <string.h>
#include <netinet/in.h>
#include <errno.h>
#include "csapp.h"

typedef struct sockaddr SA;

int init_open_listenfd(int port)
{
	int listenfd,optval = 1;
	struct sockaddr_in serveraddr;
	
	if((listenfd = socket(AF_INET,SOCK_STREAM,0)) < 0){
		char * mesg = strerror(errno);
		printf("Mesg:%s\n",mesg);
		return -1;
	}
	
	if(setsockopt(
		listenfd,
		SOL_SOCKET,
		SO_REUSEADDR,
		(const void *)&optval,
		sizeof(int)
	) < 0){
		char * mesg = strerror(errno);
		printf("Mesg:%s\n",mesg);
		return -1;
	}
	
	bzero((char *)&serveraddr,sizeof(serveraddr));
	serveraddr.sin_family = AF_INET;
	serveraddr.sin_addr.s_addr = htonl(INADDR_ANY);//设置为INADDR_ANY 会监听所有网卡的port端口
	serveraddr.sin_port = htons((unsigned short)port);
	
	if(bind(listenfd,(SA *)&serveraddr,sizeof(serveraddr)) < 0){
		char * mesg = strerror(errno);
		printf("Mesg:%s\n",mesg);
		return -1;
	}
	
	if(listen(listenfd,LISTENQ) < 0){
		char * mesg = strerror(errno);
		printf("Mesg:%s\n",mesg);
		return -1;
	}
	return listenfd;
	
}

int main()
{
	int listenfd,connfd,clientlen;
	struct sockaddr_in clientaddr;
	struct hostent *hp;
	
	listenfd = init_open_listenfd(8899);
	
	if(listenfd < 0){
		printf("Mesg: error\n");
		exit(0);
	}
	
	bzero((char *) &clientaddr,sizeof(clientaddr));
	
	while(1){
		clientlen = sizeof(clientaddr);
		if((connfd = accept(listenfd,(SA *)&clientaddr,&clientlen)) < 0){
			char * mesg = strerror(errno);
			printf("Mesg:%s\n",mesg);
			exit(0);
		}
		
		if((hp = gethostbyaddr((char *)&clientaddr.sin_addr.s_addr,sizeof(clientaddr.sin_addr.s_addr),AF_INET)) == NULL){
			char * mesg = strerror(errno);
			printf("Mesg:%s\n",mesg);
			exit(0);
		}
		printf("server connected to %s (%s) \n",hp->h_name,inet_ntoa(clientaddr.sin_addr));
		
		size_t n;
		char buf[1024];
		rio_t rio;
		
		Rio_readinitb(&rio,connfd);
		
		while((n = Rio_readlineb(&rio,buf,1024)) != 0){
			printf("server received %d bytes \n",n);
			Rio_writen(connfd,buf,n);
		}
		
		close(connfd);
	}
	
}