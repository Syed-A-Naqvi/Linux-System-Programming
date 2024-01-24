#include<stdio.h>
#include<unistd.h>
#include<sys/wait.h>


int main(int argc, char const *argv[])
{
    char const *e1 = argv[1];
    char const *e2 = argv[2];

    if (fork() == 0){
        //child process messages
        printf("I am a child process with id: %d\n", getpid());
        printf("I will execute: %s\n",e1);
        printf("Below are the results for: %s\n\n",e1);
        
        //replacing child process memory space with a new program
        char *args[] = {NULL, NULL};
        execvp(e1,args);        
    }

    else{
        //the parent will wait for child to terminate before proceeding
        //exit status of child stored in status
        int status;
        wait(&status);

        //parent process messages
        printf("\nI am a parent process with id: %d\n", getpid());
        printf("I will execute: %s\n",e2);
        printf("Below are the results for: %s\n\n",e2);
        
        //replacing child process memory space with a new program
        char* args[] = {NULL, NULL};
        execvp(e2,args);
    }

    return 0;
}

