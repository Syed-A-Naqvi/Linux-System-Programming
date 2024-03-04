#include<stdio.h>
#include<string.h>


#define SIZE 3

// checking for a page hit
int isPageHit(int *pageFrames, int page){
    
    for (size_t i = 0; i < SIZE; i++){
        if (pageFrames[i] == page){
            return 1;
        }
    }

    return 0;
}

int main(int argc, char const *argv[])
{
    int pageFrames[SIZE];
    // page sequence
    int pages[SIZE] = {1, 1, 1};
    int pageFault = 0;

    memset(pageFrames, -1, sizeof(pageFrames));
    
    for (size_t i = 0; i < SIZE; i++)
    {
        if(!isPageHit(pageFrames,pages[i])){
            pageFault ++;
            printf("Pagefault occurred! Page %d not found in memory. Adding to memory\n", pages[i]);
            pageFrames[i] = pages[i];
        }
        else{
            printf("No page fault! Page %d found in memory.\n", pages[i]);
        }
    }
    

    return 0;
}
