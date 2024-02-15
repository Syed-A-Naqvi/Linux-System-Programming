#include<stdio.h>
#include<unistd.h>

int main(int argc, char const *argv[])
{
    // prints hello and then forks but there is nothing for the child process to do
    // this program only prints hello world one time
    // printf("hello world\n");
    // fork(); 

// -----------------------------------------------------------------------------

    // one child and parent after this point
    // fork();

    // both parent and child create one more child each
    // fork();

    // four total processes print hello world
    // printf("hello world\n");

// -----------------------------------------------------------------------------

    // printing hello 14 times be calling fork and printing at specific points
    
    // this will create 8 processes
    
    // first two processes print
    fork();
    printf("hello world, printed by PID: %d\n", getpid());

    // an additional 4 prints
    fork();
    printf("hello world, printed by PID: %d\n", getpid());

    // an additional 8 prints
    fork();
    printf("hello world, printed by PID: %d\n", getpid());

    return 0;
}