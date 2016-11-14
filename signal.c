#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <sys/types.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>

void handler(int sig)
{
	int pid;
	
	// printf("handler \n");
	// //waitpid阻塞
	// while((pid = waitpid(-1,NULL,0)) > 0){
	// 	printf("get child : %d \n",pid);
	// }
	//
	// sleep(2);
	
	if((pid = waitpid(-1,NULL,0)) < 0){
		printf("waitpid() error \n");
		exit(0);
	}
	
	printf("get child : %d \n",pid);
	sleep(2);
	return;
}

int main()
{
	int i;
	
	if(signal(SIGCHLD,handler) == SIG_ERR){
		printf("error \n");
		exit(0);
	}
	
	for(i = 0; i < 3; i++)
	{
		if(fork() == 0){
			printf("Hello from child : %d \n",(int)getpid());
			sleep(1);
			exit(0);
		}
	}
	while(1)
		;
	exit(0);
}