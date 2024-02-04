#include<stdlib.h>
#include<stdio.h>
#include<unistd.h>
#include<pthread.h>
#include<semaphore.h>

// semaphores for locking critical sections
sem_t rw_mutex, mutex;

// data is the data being read and written to
// reader_count will keep track of all the current readers
int data = 0, reader_count = 0;

// this function will be executed by reader thread (readers or consumers)
void* reader_function(void* tid){

}

// this function will be executed by writer thread (writers or producers)
void* writer_function(void* tid){

}

int main(int argc, char const *argv[])
{

    // initializing both semaphores
    // we provide a pointer to the semaphore itself
    // 0 means the semaphore is thread-local (only shared by threads in the current process)
    // the final 1 refers to the initial value of the semaphores (1 = binary semaphore(mutex), >=1 is a counting semaphore)
    sem_init(&mutex, 0, 1);
    sem_init(&rw_mutex, 0, 1);

    // two arrays where each element is of the pthread data type (pthread_t may be an integer or a more complex data type)
    // pthread_t variables are used to store thread identifiers which are created upon execution of pthread_create()
    // these identifiers can be used to refer to and perform operations on a specific thread later
    pthread_t readers[5], writers[5];

    // these arrays will store a simple thread id
    int reader_ids[5], writer_ids[5];

    // creating the threads
    for (int i = 0; i < 5; i++)
    {
        reader_ids[5] = i;
        writer_ids[5] = i;
        // int = 32 bit, intptr_t = sizeof(void*) which may be greater than 32bit
        // so when passing casting between pointers and integers best to use (void*)(intptr_t)integerVariable
        pthread_create(readers[i], NULL, reader_function, &reader_ids[i]);
        pthread_create(writers[i], NULL, writer_function, &writer_ids[i]);

    }
        


    return 0;
}
