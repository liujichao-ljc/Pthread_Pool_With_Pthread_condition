#include <stdio.h>
#include <pthread.h>
#include <unistd.h>

pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t cond = PTHREAD_COND_INITIALIZER;

void *call_back(void *arg)
{
	printf("thread 0x%x is working...\n",(int)pthread_self());
	sleep(10);
	return NULL;
}

void *pthread_fun(void *arg)
{
	pthread_mutex_lock(&mutex);
	for(;;)
	{
		printf("thread 0x%x is cteate to waiting for work....\n",(int)pthread_self());
		pthread_cond_wait(&cond,&mutex);
		pthread_mutex_unlock(&mutex);
		call_back(NULL);
	}
	pthread_mutex_unlock(&mutex);
}

void main()
{
	int i=0;
	int j=0;
	for(i=0;i<8;i++)//创建8个线程，并等待
	{
		pthread_t tid;
		pthread_create(&tid,NULL,pthread_fun,NULL);
	}

	sleep(1);
	for(j=0;j<15;j++)//唤醒15次线程，相当于任务到来
	{
		pthread_mutex_lock(&mutex);
		printf("wake up...%d..\n",j);
		pthread_cond_signal(&cond);
		pthread_mutex_unlock(&mutex);
		sleep(1);
	}
}
