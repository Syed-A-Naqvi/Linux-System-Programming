#include<stdio.h>
#include<stdlib.h>
#include<stdbool.h>
#include<time.h>



#define NUM 5

struct Process {
    int PID;
    int burst_time;
    int priority;
};

// utility function, determines if provided priority is unique
bool is_unique_priority(struct Process* processs, int priority, int current_index){
    for (int i = 0; i < current_index; i++)
    {
        if (processs[i].priority == priority){
            return false;
        }
    }
    return true;
}

// process comparison function
int cmp(const void* a, const void* b){
    
    const struct Process* pa = (const struct Process*)a;
    const struct Process* pb = (const struct Process*)b;
    
    if (pa->priority < pb->priority){
        return -1;
    }
    else if (pa->priority > pb->priority)
    {
        return 1;
    }
    else{
        return 0;
    }
}

int main(int argc, char const *argv[])
{
    // seeding random number generator with current time
    srand(time(NULL));

    // creating an array of 5 processes
    struct Process processes[NUM];
    
    // populating the array
    for (int i = 0; i < NUM; i++){
        processes[i].PID = i + 1;
        processes[i].burst_time = (rand()%10) + 1;
        // temporary priority
        processes[i].priority = 0;   
    }

    // randomizing the priority of each process in the array
    for (int i = 0; i < NUM; i++)
    {
        int priority;

        do
        {
            priority = (rand()%5) + 1;
        } while (!is_unique_priority(processes, priority, i));
                  
        processes[i].priority = priority; 
    }

    // printing the processes before sorting ----------------------------------------------------------------------
    printf("Before sorting ...\n\n");
    printf("Process ID\tBurst Time\tPriority\n");
    for (int i = 0; i < NUM; i++)
    {
        printf("%d         \t%d         \t%d\n", processes[i].PID, processes[i].burst_time, processes[i].priority );
    }
    // ------------------------------------------------------------------------------------------------------------


    // sorting processes by priority in ascending order -----------------------------------------------------------
    qsort(processes, NUM, sizeof(struct Process), cmp);
    // ------------------------------------------------------------------------------------------------------------

    // calculating and displaying the wait time and turnaround times for all processes as well as the average wait time
    int wait_time = 0, turnaround_time = 0;
    float avg_wait_time = 0;
    printf("\n\nAfter sorting ...\n\n");
    printf("Process ID\tBurst Time\tPriority\tWait Time\tTurnaround Time\n");
    for (int i = 0; i < NUM; i++)
    {
        // current wait time is the previous turnaround time
        wait_time = turnaround_time;

        // current turnaround time is the current wait time + current burst time
        turnaround_time = processes[i].burst_time + wait_time;

        printf("%d         \t%d         \t%d       \t%d        \t%d\n",
            processes[i].PID, processes[i].burst_time, processes[i].priority, wait_time, turnaround_time);
        
        // summing all wait times to average later
        avg_wait_time = avg_wait_time + wait_time;
    }

    printf("Average Wait Time: %.2f\n", avg_wait_time/NUM);
    // ------------------------------------------------------------------------------------------------------------


    return 0;
}
