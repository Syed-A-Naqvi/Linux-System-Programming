#include<stdio.h>
#include<string.h>
#include<stdbool.h>

#define SIZE 4 // number of pages
#define QUEUE_SIZE 3 // max. size of the queue

int front = -1; // points to the front of the queue
int rear = -1; // points to the back of the queue
int pageFrames[SIZE];
int pageFaults = 0;

bool isEmpty(){
    return rear == -1;
}

bool isFull(){
    return rear == QUEUE_SIZE - 1;
}

void enqueue(int item){
    // Fill in this method
    rear++;
    pageFrames[rear] = item;
    printf("%d inserted into the queue.\n", item);
}

int dequeue(){
    // Fill in this method
    int i = front;
    printf("%d removed from the queue.\n", pageFrames[i]);
    while (i != rear)
    {
        pageFrames[i] = pageFrames[i+1];
        i++;
    }
    rear--;
}

bool isPageHit(int pageNumber){

    for (int i = 0; i < SIZE; i++)
    {
        if(pageFrames[i] == pageNumber){
            return true;
        }
    }

    return false;
}

void managePageFrames(int pageNumber){
    // Fill in this method
    if(isEmpty()){
        front = 0;
        enqueue(pageNumber);
    }
    else{

        if(isFull()){
            dequeue(pageNumber);
            enqueue(pageNumber);
        }
        else{
            enqueue(pageNumber);
        }
        
    }
    

}

int main(){
    
    int pages[SIZE] = {1, 2, 3, 4}; // case 1
    // int pages[SIZE] = {1, 1, 2, 3}; // case 2
    // int pages[SIZE] = {1, 2, 2, 3}; // case 3
    // int pages[SIZE] = {1, 1, 1, 1}; // case 4
    memset(pageFrames, -1, sizeof(pageFrames));
    
    for (int i = 0; i<SIZE; i++){
        if (!isPageHit(pages[i])){
            pageFaults++;
            managePageFrames(pages[i]);
        }
    }

    printf("\nTotal number of page faults: %d\n", pageFaults);
    
    return 0;
}