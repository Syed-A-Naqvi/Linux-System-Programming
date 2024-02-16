#include<stdio.h>
#include<stdlib.h>
#include<pthread.h>
#include<unistd.h>

void * thread_function(void *args)
{
	printf("I am a new thread with ID:%lu,\n", pthread_self());
	sleep(10);
}

int main()
{

	printf("The ID Of the main thread: %lu \n", pthread_self());
	
	pthread_t tid;
	int thread_status;

	pthread_create(&tid, NULL, &thread_function, NULL);
	pthread_cancel(tid);
	pthread_join(tid, (void *)&thread_status);

	if (thread_status == 0) 
	{
    	printf("Thread is still running\n");
	} 
	else 
	{
    	printf("Thread has terminated\n");
	}

	return 0;
}