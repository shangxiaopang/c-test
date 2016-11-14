#include "cs-code/include/csapp.h"

#define MAXARGS   128

struct job_{
	int id;//ID
	int status;//Runing Status
	char cmdline[MAXLINE];//Run cmdline
};

int main()
{
	char cmdline[MAXLINE];
	char bu[MAXLINE];
	char *argv[MAXARGS];
	char *str;
	char *buf;
	int argc,bg;
	pid_t pid;
	
	struct job_ job[10];
	
	while(1){
		
		printf("> ");  
		
		fgets(cmdline, MAXLINE, stdin);
	
		if (feof(stdin)){
			exit(0);
		}
	
		strcpy(bu, cmdline);
	
		buf = bu;
	
		buf[strlen(buf)-1] = ' ';
	
		//忽略开头空格
	    while (*buf && (*buf == ' ')){
	    	buf++;
	    }
	
		argc = 0;
		while(str = strchr(buf,' ')){
		
			argv[argc++] = buf;
			*str = '\0';
			buf = str + 1;
			while (*buf && (*buf == ' ')){
				buf++;
			}
		}
		
		//runing at background
		if(*argv[argc-1] == '&'){
			bg = 1;
			argv[argc-1] = NULL;
		}
		pid = fork();
		if (pid == 0) {
			
			if(!strcmp(argv[0], "jobs")){
				printf("jobs \n");
				exit(0);
			}else{
				printf("%d %s", pid, cmdline);
				exit(0);
			}
		    // if (execve(argv[0], argv, environ) < 0) {
// 				printf("%s: Command not found.\n", argv[0]);
// 				exit(0);
// 		    }
		}
		
		if(bg){
		    int status;
		    if (waitpid(pid, &status, 0) < 0){
		    	printf("waitfg: waitpid error");
		    	exit(0);
		    }
		}else{
			printf("%d %s", pid, cmdline);
		}
		
	}
}



