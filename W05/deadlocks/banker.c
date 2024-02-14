#include<stdio.h>
#include<stdlib.h>
#include<stdbool.h>


#define NO_OF_PROCESSES 5

// this refers to the number of different TYPES of resources
// ex: 4 keyboards, 2 printers, 10 scanners
#define NO_OF_RESOURCES 3

struct Process {
    int pid; // process id
    int allocations[10]; // currently allocated number of resource instances
    int maximum[10]; // maximum required resource instance allocations for termination
    int remaining[10]; // allocations remaining in order to reach completion levels
    bool completed; // indicates if process has been completed
};

int main(int argc, char const *argv[])
{
    struct Process proc[NO_OF_PROCESSES] = {
        
        // so this process has id 0, 0 allocations of resource 1, 1 allocation of resource 2 ...
        // requires a maximum of 7,5,3 allocations for each respective resource in order to terminate
        // still needs 7,4,3 allocaitons in order to reach max levels and terminate 
        {0, {0,1,0}, {7,5,3}, {7,4,3} },

        {1, {2,0,0}, {3,2,2}, {1,2,2} },
        {2, {3,0,2}, {9,0,2}, {6,0,0} },
        {3, {2,1,1}, {2,2,2}, {0,1,1} },
        {4, {0,0,2}, {4,3,3}, {4,3,1} }
    };

    int available[NO_OF_RESOURCES] = {3,3,2};
    // storing the sequence of process execution
    int sequence[NO_OF_PROCESSES];
    int index = 0;

    // printing available resources
    printf("\nAvailable Resources: R0=%d, R1=%d, R2=%d\n", available[0], available[1], available[2]);

    // for each process, printing the pid, allocations, maximums, and needed
    printf("\nProcess \t Resource # \t Allocations \t Maximum \t Needed\n");
    printf("----------------------------------------------------------------------------\n");
    for (int i = 0; i < NO_OF_PROCESSES; i++)
    {
        for (int j = 0; j < NO_OF_RESOURCES; j++)
        {
            printf("P%d \t\t %d \t\t %d \t\t %d \t\t %d\n", proc[i].pid, j, proc[i].allocations[j], proc[i].maximum[j], proc[i].remaining[j]);
        }
        printf("----------------------------------------------------------------------------\n");
    }

    // Bankers Algorithm

    bool pass_again = false;
    do
    {
        // we will pass over the process list over and over until pass_again remains false and all
        // processes have been completed
        pass_again = false;

        // iterate over each process
        for (int i = 0; i < NO_OF_PROCESSES; i++)
        {
            // if the process has not been completed execute branch
            if (!proc[i].completed)
            {

                // we assume the process can be completed
                proc[i].completed = true;

                // iterate over each resource type for current process
                for (int j = 0; j < NO_OF_RESOURCES; j++)
                {   
                    // if there are not enough instances available to satisfy required amount for any resource
                    // process cannot be completed, mark as incomplete again
                    if (proc[i].remaining[j] > available[j])
                    {   
                        // set process back to incomplete
                        proc[i].completed = false;
                        // we will need to pass over the process list again
                        pass_again = true;
                        break;
                    }                
                }

                // if process was not marked incomplete again by previous loop, we know it can be completed
                if (proc[i].completed == 1){

                    // add to sequence
                    sequence[index] = i;
                    index++;
                    printf("\nExecuting P%d and deallocating resources...\n", proc[i].pid);

                    // return all resources currently help by process back to the os
                    for (int x = 0; x < NO_OF_RESOURCES; x++)
                    {
                        available[x] += proc[i].allocations[x];
                    }
                    printf("Updated Resources: R0=%d, R1=%d, R2=%d\n", available[0], available[1], available[2]);

                }
            
            }
        
        }
    } while (pass_again);
        

    return 0;
}

