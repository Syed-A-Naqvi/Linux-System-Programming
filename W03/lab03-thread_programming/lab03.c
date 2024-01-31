#include<stdio.h>
#include<stdlib.h>
#include<pthread.h>
#include<unistd.h>


// function will be executed by the child thread
// will create a dynamically allocated array using malloc
void* create_array(void* args){
    
    // displaying child thread ID
    printf("\nI am the child thread with ID: %lu \n", pthread_self());

    // storing array size
    int* size = args;

    // creating the dynamically allocated array 
    int* numbers = malloc((*size) * sizeof(int));

    // populating the array
    // returns -1 if the array size is negative
    if(*size < 0){
        printf("Must pass valid array size to program. Child thread terminated after unsuccessful termination.\n");
        return (void*)-1;
    }
    else{
        for (int i = 0; i < *size; i++)
        {
            printf("Inserted %d into the array using the child thread...\n", i);
            numbers[i] = i;
        }
    }

    // returning a pointer to the dynamically allocated child array
    printf("Child thread terminated after successful execution.\n");
    return numbers;
}

int main(int argc, char const *argv[])
{

    // displaying parent thread message
    printf("I am the parent thread with ID: %lu\n", pthread_self());

    // exiting if no argument given
    if (argv[1] == NULL){
        printf("No array size provied. No child thread created. Parent thread terminated after unsuccessful execution.\n");
        return -1;
    }

    // this will store the id of the newly created child thread
    pthread_t tid;

    // storing desired array size from user
    int size = atoi(argv[1]);

    // here we are creating a thread, it will use a pointer to tid (&tid) to update the threadid stored in tid
    // first NULL is because we are not passing any thread attributes
    // we then pass a pointer to the function that the child thread will execute &create_array
    // the last argument is the desired array size that our child thread will allocate
    pthread_create(&tid, NULL, &create_array, &size);

    // this will store the return value of the child thread
    int* numbers;

    //waiting for the thread with id tid to terminate and storing a pointer to the dynamically allocated array in the numbers pointer
    pthread_join(tid, (void**)&numbers);

    // printing the array
    if(numbers != (int*)-1){
        printf("\nDisplaying elements of the array in the parent thread...\n");        
        for (int i = 0; i < size; i++)
        {
            printf("Element at index %d: %d\n", i, numbers[i]);
        }

        //deallocating dynamic array
        free(numbers);

    }

    // final status message
    printf("\nParent thread terminated after successful execution.\n");
    

    return 0;
}
