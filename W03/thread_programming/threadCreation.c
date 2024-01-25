#include<stdio.h>
#include<stdlib.h>
#include<pthread.h>
#include<unistd.h>


// task/function that will be executed by the child thread
void* thread_function(void* args){
    printf("This is the child thread.\n");
    printf("The ID of the child thread is: %lu \n", pthread_self());
    return (void*)(__intptr_t)0;
}

int main(int argc, char const *argv[])
{

    printf("This is the main thread.\n");
    printf("The ID of the main thread is: %lu \n", pthread_self());

    // this will store the id of the newly created thread
    pthread_t tid;

    // here we are creating a thread, it will use a pointer to tid (&tid) to update the threadid stored in tid
    // first NULL is because we are not passing any thread attributes
    // we then pass a pointer to the function that the child thread will execute &thread_funciton
    // the last NULL is there because we are passing no arguments to thread_function
    pthread_create(&tid, NULL, &thread_function, NULL);

    // this will store the exit status of the thread
    void* texit;

    //waiting for the thread with id tid to terminate and storing the exit status in texit
    pthread_join(tid, &texit);

    // final status message
    printf("Thread with ID %lu terminated with exit status %ld\n", tid, (__intptr_t)texit);

    return 0;
}
