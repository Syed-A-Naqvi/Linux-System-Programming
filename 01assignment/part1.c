#include<stdio.h>
#include<stdlib.h>
#include<time.h>
#include<pthread.h>

void* incremental_sum(void* arg){

    __intptr_t* num = (__intptr_t*)arg;
    
    // Displaying thread ID and purpose
    printf("Child Thread 1 - ID: %lu\n", pthread_self());
    printf("Calculating incremental sum of numbers from 1 to %ld\n\n", *num);

    // Calculating and returning incremental sum of passed value
    __intptr_t inc_sum = 0;

    for (__intptr_t i = 1; i < *num + 1; i++)
    {
        inc_sum += i;
    }
    
    return (void*) inc_sum;

}

void* incremental_even_sum(void* arg){

    __intptr_t* num = (__intptr_t*)arg;
    
    // Displaying thread ID and purpose
    printf("Child Thread 2 - ID: %lu\n", pthread_self());
    printf("Calculating incremental sum of even numbers from 1 to %ld\n\n", *num);

    // Calculating and returning incremental sum of even numbers up to passed value (inclusive)
    __intptr_t inc_sum = 0;

    for (__intptr_t i = 2; i < *num + 1; i+=2)
    {
        inc_sum += i;
    }
    
    return (void*) inc_sum;

}

int main(int argc, char const *argv[])
{
    // used to calculate execution time in terms of clock ticks
    clock_t start, end;
    double time_elapsed;

    // recording start time
    start = clock();

    // Displaying thread ID and purpose
    printf("\nParent thread - ID: %lu\n", pthread_self());
    printf("Creating child threads for calculating incremental sums...\n");
    printf("Calculating ratio...\n\n");

    // storing command line argument
    __intptr_t value = atoi(argv[1]);

    // will store child ids
    pthread_t child1;
    pthread_t child2;

    // will store child results
    void* inc_sum;
    void* inc_sum_even;
    
    // creating child threads
    pthread_create(&child1, NULL, &incremental_sum, (void*) &value);
    pthread_create(&child2, NULL, &incremental_even_sum, (void*) &value);

    // joining child threads
    pthread_join(child1, &inc_sum);
    pthread_join(child2, &inc_sum_even);

    // calculating ratio
    double ratio = (double)(__intptr_t)inc_sum / (__intptr_t)inc_sum_even;
    
    // displaying results of final calculation
    printf("incremental sum = %ld\n", (__intptr_t)inc_sum);
    printf("incremental even sum = %ld\n\n", (__intptr_t)inc_sum_even);
    printf("incremental sum / incremental even sum = %f\n\n", ratio);

    // recording end time
    end = clock();
    // calculating total elapsed time
    time_elapsed = (double)(end - start) / CLOCKS_PER_SEC;
    // displaying elapsed time
    printf("Execution time = %fs\n", time_elapsed);

    return 0;
}
