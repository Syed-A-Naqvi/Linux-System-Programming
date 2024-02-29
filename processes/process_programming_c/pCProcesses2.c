#include<stdio.h>
#include<unistd.h>

int main()
{
    

    int i;
    // i is the value of the child PID when in the parent process
    // i is 0 in the child process
    // fork returns child PID in parent and 0 in child
    i = fork();

    //printing results
    printf("Process ID:%d\n", getpid());
    printf("i value:%d\n", i);

    return 0;
}

