#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>

void *thread(void *vargp);
volatile int cnt = 0;

int main()
{
	int niters = 1000000;
	pthread_t tid1,tid2;
	
	pthread_create(&tid1,NULL,thread,&niters);
	pthread_create(&tid2,NULL,thread,&niters);
	
	pthread_join(tid1,NULL);
	pthread_join(tid2,NULL);
	
	if(cnt != (2 * niters)){
		printf("BOOM! cnt=%d \n",cnt);
	}else{
		printf("OK! cnt=%d \n",cnt);
	}
	
}

void *thread(void *vargp)
{
	int i,niters = *((int *)vargp);
	
	for(i = 0; i < niters;i++){
		cnt++;
	}
	
	return NULL;
}

