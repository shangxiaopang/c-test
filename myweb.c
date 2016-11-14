#include "csapp.h"
#include <ctype.h>

typedef struct sockaddr SA;

void echoError();
void doit(int fd);
void params();
void clientMsg(int fd,char *msg);
int openListenFd(int port);
void ioAccept();
void selectAccept();

ssize_t sRio_readlineb(rio_t *rp, void *usrbuf, size_t maxlen) 
{
    ssize_t rc;

    if ((rc = rio_readlineb(rp, usrbuf, maxlen)) < 0){
    	return 0;
    }
    return rc;
} 



int main()
{
	selectAccept();
}

void ioAccept()
{
	int listenFd,connFd,clientLen,flags,i;
	struct sockaddr_in clientAddr;
	char buf[1024];
	
	if((listenFd = openListenFd(8898)) < 0){
		printf("Mesg: open listen error\n");
		exit(0);
	}
	
	printf("listen \n");
	
	//初始化
	bzero((char *) &clientAddr,sizeof(clientAddr));
	
	while(1){
		
		clientLen = sizeof(clientAddr);
		//阻塞等待客户端连接
		if((connFd = accept(listenFd,(SA *) &clientAddr,&clientLen)) < 0){
			echoError("accept");
			exit(0);
		}
		
		//flags = fcntl(connFd, F_GETFL, 0);
		//fcntl(connFd, F_SETFL,flags | O_NONBLOCK);
		printf("accept-start \n");
		
		for(i = 0;i < 1024;i++){
			read(connFd,buf,1);
			printf("%s \n",buf);
		}
		
		printf("accept-end \n");
		
		//doit(connFd);
		//clientMsg(connFd);
		//close(connFd);
		
	}
}

void selectAccept()
{
	int listenFd,connFd,clientLen,flags,selectCount;
	struct sockaddr_in clientAddr;
	fd_set read_set,ready_set;
	char buf[1024];
	
	if((listenFd = openListenFd(8898)) < 0){
		printf("Mesg: open listen error\n");
		exit(0);
	}
	
	//初始化
	bzero((char *) &clientAddr,sizeof(clientAddr));
	
	FD_ZERO(&read_set);
	FD_SET(listenFd,&read_set);
	
	clientLen = sizeof(clientAddr);
	
	while(1){
		ready_set = read_set;
		if((selectCount = select(listenFd+1,&ready_set,NULL,NULL,NULL)) == -1){
			echoError("select");
			exit(0);
		}
		
		printf("select:%d \n",selectCount);
		
		if(FD_ISSET(listenFd,&ready_set)){
			
			//阻塞等待客户端连接
			if((connFd = accept(listenFd,(SA *) &clientAddr,&clientLen)) < 0){
				echoError("accept");
				exit(0);
			}
			
			flags = fcntl(connFd, F_GETFL, 0);
			fcntl(connFd, F_SETFL,flags | O_NONBLOCK);
			
			doit(connFd);
			close(connFd);
		}
	}
}

void doit(int connFd)
{
	int is_static,is_null = 0,contentLength;
	struct stat sbuf;
	char buf[MAXLINE],method[1024],uri[1024],version[1024],params[1024],key[1024],value[1024];
	char filename[MAXLINE];
	
	
	size_t n;
	rio_t rio;
	Rio_readinitb(&rio,connFd);
	
	sRio_readlineb(&rio,buf,MAXLINE);
	sscanf(buf, "%s %s %s", method, uri, version);
	
	if(strcmp(method,"") || strcmp(uri,"") || strcmp(version,"")){
		clientMsg(connFd,"data is error");
	}else{
		printf("get request \n");
	
		while((n = sRio_readlineb(&rio,buf,MAXLINE)) != 0){
		
			sscanf(buf, "%[a-zA-z-]: %[^\0\r\n]", key, value);
		
			if(strcasecmp(key, "Content-Length") == 0){
				contentLength = atoi(value);
			}
		
			if(strcmp(buf, "\r\n") == 0){
			
				printf("method:%s \n",method);
			
				if (strcasecmp(method, "POST") == 0){
				
					memset(params,0,sizeof(params));
				
					Rio_readnb(&rio,params,contentLength);
					printf("params:%s \n",params);
				}
			
				return;
			}
			
			printf("%s: %s \n",key,value);
		}
	}
	
}

void clientMsg(int fd,char *msg)
{
	char buf[MAXLINE],body[MAXLINE];
	
	sprintf(body,msg);
	
	sprintf(buf,"HTTP/1.1 200 msg \r\n");
	Rio_writen(fd,buf,strlen(buf));
	
	sprintf(buf,"Content-type: text/html \r\n");
	Rio_writen(fd,buf,strlen(buf));
	
	sprintf(buf,"Content-length:%d \r\n\r\n",(int)strlen(body));
	
	Rio_writen(fd,buf,strlen(buf));
	Rio_writen(fd,body,strlen(body));
}

void echoError(char *tag)
{
	char * mesg = strerror(errno);
	printf("%s Mesg:%s\n",tag,mesg);
}

int openListenFd(int port)
{
	int listenFd,optval = 1;
	struct sockaddr_in serverAddr;
	//AF_INET ip4
	if((listenFd = socket(AF_INET,SOCK_STREAM,0)) < 0){
		echoError("socket");
		return -1;
	}
	
	if(setsockopt(
		listenFd,
		SOL_SOCKET,
		SO_REUSEADDR,
		(const void *)&optval,
		sizeof(int)
	)){
		echoError("setsockopt");
		return -1;
	}
	
	bzero((char *)&serverAddr,sizeof(serverAddr));
	serverAddr.sin_family = AF_INET;
	serverAddr.sin_addr.s_addr = htonl(INADDR_ANY);//设置为INADDR_ANY 会监听所有网卡的port端口
	serverAddr.sin_port = htons((unsigned short)port);
	
	if(bind(listenFd,(SA *)&serverAddr,sizeof(serverAddr)) < 0){
		echoError("bind");
		return -1;
	}
	//监听端口
	if(listen(listenFd,LISTENQ) < 0){
		echoError("listen");
		return -1;
	}
	return listenFd;
}

