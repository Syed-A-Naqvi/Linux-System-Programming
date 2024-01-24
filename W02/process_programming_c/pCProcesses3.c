#include<stdio.h>
#include<unistd.h>
#include<stdlib.h>


//prints odd numbers in [start,end]
void show_odd(int start, int end){
    printf("The child prints only odd numbers:\n");
    for (int i = start; i < end+1; i++){
        if (i%2 != 0){
            printf("C - %d\n",i);
        }
    }
}
//prints even numbers in [start,end]
void show_even(int start, int end){
    printf("The parent prints only even numbers:\n");
    for (int i = start; i < end+1; i++){
        if (i%2 == 0){
            printf("P - %d\n",i);
        }
    }
}

int main(int args, char *argv[])
{
    //accepting start and end bounds from terminal
    int start = atoi(argv[1]);
    int end = atoi(argv[2]);

    printf("printing numbers from %d to %d\n", start, end);

    //assigning i with pid of child in the case calling process is parent
    //assigning i with 0 in the case calling process is child 
    int i;
    i = fork();

    //changing what is printing based on whether calling process is a parent or child
    if (i == 0){
        printf("I am a child. PID = %d\n", getpid());
        show_odd(start, end);
    }
    else{
        printf("I am a parent. PID = %d\n", getpid());
        show_even(start, end);
    }
    return 0;
}
