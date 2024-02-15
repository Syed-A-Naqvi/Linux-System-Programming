#include<stdio.h>
#include<unistd.h>

int main(int argc, char const *argv[])
{
    int i = fork();

    if (i == -1)
    {
        printf("something went wrong, no child process created.\n");
    }
    else if (i == 0)
    {
        printf("I am just a child. My process ID is %d\n", getpid());
    }
    else{
        printf("I am the daddy, my process id is %d and my childs id is %d.\n", getpid(), i);
    }
    
    
    return 0;
}

