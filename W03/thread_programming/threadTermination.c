#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<pthread.h>

void* thread_function(void* args){
    
    printf("I am a child thread with ID: %lu\n", pthread_self());

    // we can make the function sleep for some seconds during which we can terminate this thread in the main thread
    // we can do this to test various termination exit statuses 
    sleep(5);

    // return type of NULL or 0 indicates termination after successful execution
    return 0;
}

int main(int argc, char const *argv[])
{
    
    //main thread print funciton
    printf("I am the main thread with ID: %lu\n", pthread_self());

    // new thread id and exit status
    pthread_t newThreadID;
    void* newThreadExitStatus;

    //creating a new thread and executing thread function
    pthread_create(&newThreadID, NULL, &thread_function, NULL);
    
    //cancelling thread execution for thread with id: newThreadID
    //if thread_function was sleeping, cancellation will occur before thread_function completes execution
    pthread_cancel(newThreadID);

    //newThreadExitStatus should now contain hex of some error code
    pthread_join(newThreadID, &newThreadExitStatus);


    if( (__intptr_t) newThreadExitStatus == 0){
        printf("Thread with ID: %lu terminated after executing successfully.\n", newThreadID);
        printf("Thread exit status: %ld\n", (__intptr_t)newThreadExitStatus);        
    }
    else{
        printf("Thread with ID: %lu terminated after executing with an error.\n", newThreadID);
        printf("Thread exit status: %ld\n", (__intptr_t)newThreadExitStatus);        
    }

    return 0;
}
