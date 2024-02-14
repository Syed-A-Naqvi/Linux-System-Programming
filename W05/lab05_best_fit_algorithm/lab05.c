#include <stdio.h>
#include <limits.h>
#include <stdbool.h>


#define PROCESS_SIZE 5
#define BLOCK_SIZE 5

// creating struct
struct Process
{
    int id;
    int size;
};

// best fit algorithm
void best_fit(int* blocks, struct Process* processes, int* allocations){
    
    // iterating over processes
    for (int i = 0; i < PROCESS_SIZE; i++)
    {
        // checking whether process should be allocated
        bool allocate = false;

        // storing index of hole closest in size to current process
        int closest_hole = 0;
        int difference = INT_MAX;

        // iterating over blocks to find hole closest in size to current process
        for (int j = 0; j < BLOCK_SIZE; j++)
        {
            if(blocks[j] >= processes[i].size){
                allocate = true;
                if ( blocks[j] - processes[i].size < difference){
                    closest_hole = j;
                    difference = blocks[j] - processes[i].size;
                }
            }
        }

        if (allocate){
            // allocating process to memory block
            allocations[i] = closest_hole;
            // updating hole size in block after allocation by subtracting process size
            blocks[closest_hole] -= processes[i].size;
        }
        
        
    }
    
}

int main(int argc, char const *argv[])
{
    // creating processes, memory blocks, and allocation array
    struct Process processes[PROCESS_SIZE] = {
        {1, 212},
        {2, 417},
        {3, 112},
        {4, 426},
        {5, 80}
    };
    int blocks[BLOCK_SIZE] = {100, 500, 200, 300, 600};
    int allocations[PROCESS_SIZE] = {-1, -1, -1, -1, -1};


    // best fit call
    best_fit(blocks, processes, allocations);

    // displaying output
    printf("Process Id \t Process Size \t Block\n");
    for (int i = 0; i < PROCESS_SIZE; i++)
    {
        printf("%d \t\t %d \t\t", processes[i].id, processes[i].size);
        if(allocations[i] == -1){
            printf(" Unallocated\n");
        }
        else{
            printf(" %d\n", allocations[i]+1);
        }
    }
    


    return 0;
}
