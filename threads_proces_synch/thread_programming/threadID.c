// at the creation of a process there will be a default thread in it. Let us return this thread ID.

#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<pthread.h>

int main(int argc, char const *argv[])
{
    // printing the thread id using an unsigned long int format specifier
    printf("The ID of the main thread: %lu \n", pthread_self());

    // can also convert thread ID into int and then show
    printf("The ID of the main thread: %d \n", (int)pthread_self());

    // can use (void*) to treat thread ID as though it was a pointer allowing us to print the id in hexadecimal
    printf("The ID of the main thread: %p \n", (void*)pthread_self());


    // printing the process ID
    printf("The ID of the parent process: %d \n", getpid());

    return 0;
}
