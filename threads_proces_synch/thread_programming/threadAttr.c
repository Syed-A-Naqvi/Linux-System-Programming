#include<stdio.h>
#include<stdlib.h>
#include<pthread.h>
#include<unistd.h>

void * thread_function(void * args)
{
	printf("I am the deteched thread\n");
}



int main(int args, char* argv[])
{

	printf("The ID Of the main thread: %lu \n", pthread_self());

	pthread_t tid;
	pthread_attr_t attr;

	pthread_attr_init(&attr);

	pthread_attr_setstacksize(&attr, 16384);
	//pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_DETACHED);

	pthread_create(&tid, &attr, &thread_function, NULL);
	
	pthread_attr_destroy(&attr);

	return 0;
}

// Above program might not show output from thread_function as main thread might complete before thread can be executed.