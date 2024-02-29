#include<stdlib.h>
#include<stdio.h>
#include<unistd.h>
#include<pthread.h>

// this function is going to assume that args will be an integer pointer
void* thread_function(void* args){

    int* end = args;

    printf("Child thread executing with ID: %lu\n", pthread_self());


    for (int i = 1; i < (*end + 1); i++)
    {
        if (i%2 == 0){
            printf("i: %d\n", i);
        }
    }
    return 0;

}

int main(int argc, char const *argv[])
{
    printf("This is the main thread with ID: %lu\n\n", pthread_self());

    pthread_t tid;
    void* tStatus;
    int end = atoi(argv[1]);

    pthread_create(&tid, NULL, &thread_function, &end);

    pthread_join(tid, &tStatus);

    if((__intptr_t)tStatus == 0){
        printf("Child process has terminated with exit status: %ld\n", (__intptr_t)tStatus);
    }
    else{
        printf("Child process has terminated with exit status: %ld\n", (__intptr_t)tStatus);
    }

    return 0;
}
