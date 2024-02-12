#include<stdio.h>
#include<stdlib.h>
#include<time.h>
#include<pthread.h>


int main(int argc, char const *argv[])
{
    // used to calculate execution time in terms of clock ticks
    clock_t start, end;
    double time_elapsed;
    // recording start time
    start = clock();


    // extract command line argument and convert to int
    size_t num = atoi(argv[1]);


    // calculating incremental sum
    size_t inc_sum = 0;
    for (size_t i = 1; i < num + 1; i++)
    {
        inc_sum += i;
    }
    // calculating incremental sum of even numbers
    size_t inc_sum_even = 0;
    for (size_t i = 0; i < num + 1; i+=2)
    {
        inc_sum_even += i;
    }


    // calculating ratio
    double ratio = (double) inc_sum / inc_sum_even;


    // displaying results of final calculation
    printf("\nincremental sum = %ld\n\n", inc_sum);
    printf("incremental even sum = %ld\n\n", inc_sum_even);
    printf("incremental sum / incremental even sum = %f\n\n", ratio);


    // recording end time
    end = clock();
    // calculating total elapsed time
    time_elapsed = (double)(end - start) / CLOCKS_PER_SEC;


    // displaying elapsed time
    printf("Execution time = %fs\n\n", time_elapsed);


    return 0;
}
