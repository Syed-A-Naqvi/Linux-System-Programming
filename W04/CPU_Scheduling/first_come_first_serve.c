#include<stdio.h>
#include<stdlib.h>

#define NUM 5

int main(int argc, char const *argv[])
{
    int burst_time[5], wait_time = 0, turn_around_time = 0;
    float avg_wait_time = 0;

    printf("Process\tBurst Time\tWait Time\tTurn Around Time\n");
    for (int i = 0; i < NUM; i++)
    {   
        burst_time[i] = rand()%10;
        printf("%d\t%d\t\t%d\t\t%d\n ", i, burst_time[i], wait_time, turn_around_time);
        avg_wait_time = avg_wait_time + wait_time;
        wait_time = wait_time + burst_time[i];
    }
    
    printf("Average Wait Time: %.2f\n", avg_wait_time/NUM);
    return 0;
}
