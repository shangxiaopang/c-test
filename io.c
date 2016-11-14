#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>
#include <errno.h>
#include <stdlib.h>
#include <ctype.h>

void testWrite()
{
	int fd;
	
	fd = open("foo.txt",O_RDWR|O_CREAT,S_IRUSR|S_IWUSR|S_IXUSR|S_IROTH|S_IWOTH|S_IXOTH);
	
	char buf[] = "abcdefg";
	char *p;
	
	p = buf;
	
	int i = sizeof(buf);
	
	while(i > 0){
		write(fd,p++,1);
		i--;
	}
	
	close(fd);
}

void testRead()
{
	int fd,error;
	
	char buf[5];
	
	fd = open("/Users/shangxiaopang/Sites/c-test/foo.txt",O_RDONLY,0);
	
	// while((error = read(fd,buf,1)) > 0){
// 		printf("%s",buf);
// 	}
	
	error = read(fd,buf,50);
	printf("%d \n",error);
	
	error = read(fd,buf,50);
	printf("%d \n",error);
	
	error = read(fd,buf,50);
	printf("%d \n",error);
	
	close(fd);
}

void testRead1()
{
	int fd1,fd2;
	char buf;
	fd1 = open("/Users/shangxiaopang/Sites/c-test/foo.txt",O_RDONLY,0);
	fd2 = open("/Users/shangxiaopang/Sites/c-test/foo.txt",O_RDONLY,0);
	
	read(fd1,&buf,2);
	read(fd2,&buf,1);
	printf("c = %c \n",buf);
	exit(0);
}

void testRead2()
{
	//共享文件
	int fd;
	char buf;
	fd = open("/Users/shangxiaopang/Sites/c-test/foo.txt",O_RDONLY,0);
	
	if(fork() == 0){
		read(fd,&buf,1);
		printf("cc = %c \n",buf);
		exit(0);
	}
	wait(NULL);
	read(fd,&buf,1);
	printf("c = %c \n",buf);
	exit(0);
}

void testRead3()
{
	//io重定向
	int fd1,fd2;
	char buf;
	fd1 = open("/Users/shangxiaopang/Sites/c-test/foo.txt",O_RDONLY,0);
	fd2 = open("/Users/shangxiaopang/Sites/c-test/foo.txt",O_RDONLY,0);
	
	read(fd2,&buf,1);
	dup2(fd2,fd1);
	read(fd1,&buf,1);
	printf("c = %c \n",buf);
	exit(0);
}

void testRead4()
{
	int fd1,fd2,fd3;
	fd1 = open("/Users/shangxiaopang/Sites/c-test/foo.txt",O_RDONLY,0);
	fd2 = open("/Users/shangxiaopang/Sites/c-test/foo2.txt",O_RDONLY,0);
	printf("fd = %d \n",fd1);
	printf("fd = %d \n",fd2);
	
	close(fd2);
	fd2 = open("/Users/shangxiaopang/Sites/c-test/foo2.txt",O_RDONLY,0);
	fd3 = open("/Users/shangxiaopang/Sites/c-test/foo2.txt",O_RDONLY,0);
	
	printf("fd = %d \n",fd2);
	printf("fd = %d \n",fd3);
	
	exit(0);
}

void testRead5()
{
	int fd1,i,er,flags;
	char buf[1024];
	fd1 = open("/Users/shangxiaopang/Sites/c-test/foo.txt",O_RDONLY,0);
	
	for(i = 0; i < 50;i++){
		er = read(fd1,&buf,2048);
		printf("%d %s %d \n",i,buf,er);
	}
}

int main()
{
	//testWrite();
	testRead5();
}