#include<stdio.h>
#include<stdlib.h>
#include<pthread.h>
#include<unistd.h>


void* thread_function(void* args){

    __intptr_t count = (__intptr_t)args;
    
    printf("I am a child thread with TID: %ld\n", pthread_self());
    
    printf("\nI will now start counting to %ld...\n", count);
    
    for (int i = 0; i < count; i++)
    {
        sleep(1);
        printf("%d...\n", i+1);
    }

    return 0;
}

int main(int argc, char const *argv[])
{
    
    __intptr_t count = atoi(argv[1]);

    printf("I am a parent thread with ID: %ld\n", pthread_self());

    pthread_t tid;
    void* status;

    pthread_create(&tid, NULL, thread_function, (void*)count);
    sleep(3);
    pthread_cancel(tid);
    pthread_join(tid, &status);

    if ( *((int*)status) == 0)
    {
        printf("Termination of thread ID:%ld was successful.\n", tid);
    }
    else{
        printf("Termination of thread ID:%ld was unsuccessful.\n", tid);

    }
    

    return 0;
}
