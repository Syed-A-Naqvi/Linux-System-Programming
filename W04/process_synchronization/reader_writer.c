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

    // extracting the id passed as an argument
    int id = *((int*)tid);

    // this mutex will protected against asynchronous updating of the reader_count variable
    sem_wait(&mutex);
    reader_count++;
    // we want to ensure that the first reader (read_count == 1) acquires rw_mutex
    // so that read and write remain mutually exclusive   
    if(reader_count == 1){
        sem_wait(&rw_mutex);
    }
    // once reader_count has been updated and the first reader locks rw_mutex, the mutex semaphore is released
    // this allows other readers to now lock mutex and update reader_count in an atomic manner
    sem_post(&mutex);

    // Critical Section
    // now that read_count has been safely updated we can read from data
    printf("Data read by thread %d is %d.\n",id, data);
    sleep(1); // so that we can better observe each of the readers read from data

    // once again we lock mutex in order to safely decrement read_count
    sem_wait(&mutex);
    reader_count--;
    // if the last reader has completed reading (reader_count == 0), we can release rw_mutex allowing writer acquisition
    if (reader_count == 0)
    {
        sem_post(&rw_mutex);
    }
    // release the lock for other readers to decrement reader_count
    sem_post(&mutex);

}

// this function will be executed by writer thread (writers or producers)
void* writer_function(void* tid){

    // extracting current thread id
    int id = * ((int*)tid);

    // locking rw_mutex before attempting to write
    sem_wait(&rw_mutex);

    // Critical Section
    printf("\nThread id: %d\n", id);
    printf("Old data: %d\n", data);
    sleep(1);
    printf("Writing new data ...\n");
    data++;
    sleep(1);
    printf("New data: %d\n", data);

    // releasing the lock so other writers can write or readers can reade
    sem_post(&rw_mutex);

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
        reader_ids[i] = i;
        writer_ids[i] = i;
        // int = 32 bit, intptr_t = sizeof(void*) which may be greater than 32bit
        // so when passing casting between pointers and integers best to use (void*)(intptr_t)integerVariable
        pthread_create(&readers[i], NULL, reader_function, &reader_ids[i]);
        pthread_create(&writers[i], NULL, writer_function, &writer_ids[i]);
    }

    // joining the threads
    for (int i = 0; i < 5; i++)
    {
        pthread_join(readers[i],NULL);
        pthread_join(writers[i],NULL);
    }

    return 0;
}
