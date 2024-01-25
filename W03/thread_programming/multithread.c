#include<stdlib.h>
#include<stdio.h>
#include<unistd.h>
#include<pthread.h>

void* even_function(void* args){

    int* end = args;

    printf("Executing even function child thread with ID: %lu\n", pthread_self());

    for (int i = 1; i < (*end) + 1; i++)
    {
        if (i%2 == 0){
            printf("i: %d\n",i);
        }
    }
    
    return 0;

}

void* odd_function(void* args){

    int* end = args;

    printf("Executing odd function child thread with ID: %lu\n", pthread_self());

    for (int i = 1; i < (*end) + 1; i++)
    {
        if (i%2 != 0){
            printf("i: %d\n",i);
        }
    }
    
    return 0;


}

int main(int argc, char const *argv[])
{
    printf("Executing main thread with ID: %lu\n\n", pthread_self());

    //creating arrays of thread ids and thread exit statuses
    pthread_t tid[2];
    void* tStatus[2];
    int end = atoi(argv[1]);

    pthread_create(&tid[0], NULL, &even_function, &end);
    pthread_create(&tid[1], NULL, &odd_function, &end);
    
    pthread_join(tid[0], &tStatus[0]);
    pthread_join(tid[1], &tStatus[1]);

    for (int i = 0; i < 2; i++){
        if( (__intptr_t)tStatus[i] == 0 ){
            printf("Child thread ID: %lu\n", tid[i]);
            printf("Exit status: %ld\n", (__intptr_t)tStatus[i]);
        }
        else{
            printf("Child thread ID: %lu\n", tid[i]);
            printf("Exit status: %ld\n", (__intptr_t)tStatus[i]);
        }
    }

    return 0;
}
