#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#include "Semaphore.h"



//functions and variables for solution
int readersCount;
struct Semaphore resource_sem1;
struct Semaphore mutexSem;

double reader_total = 0;
int reader_count = 0;

double writer_total = 0;
int writer_count = 0;

double both_total = 0;
int both_count = 0;

//errors if name is only reader
void *reader1(void *arg) {
    clock_t start = clock();
    int thread_id = *((int *)arg);
    printf("Reader %ld is waiting.\n", (unsigned long)pthread_self());
    new_wait(&mutexSem); //lock mutex and increase reader count
    readersCount++;
    if (readersCount == 1){
        new_wait(&resource_sem1); //if the first reader, lock resource as well
    }
    new_signal(&mutexSem); //unlock mutex for other readers to use
    printf("Reader %ld is reading.\n", (unsigned long)pthread_self());

    //reading the resource
    sleep(1);

    clock_t end = clock();
    new_wait(&mutexSem); //lock mutex to decrease reader count
    readersCount--;
    if (readersCount == 0) { //last reader unlocks resource
        new_signal(&resource_sem1);
    }
    new_signal(&mutexSem); //unlock mutex
    printf("Reader %ld has finished reading.\n", (unsigned long)pthread_self());

    double reader_tat = (double)(end - start) / CLOCKS_PER_SEC;
    reader_total += reader_tat;
    both_total += reader_tat;
    reader_count++;
    both_count++;

    return NULL;
}

//errors if name is only writer
void *writer1(void *arg) {
    clock_t start = clock();
    int thread_id = *((int *)arg);
    //wait to write
    printf("Writer %ld is waiting.\n", (unsigned long)pthread_self());
    new_wait(&resource_sem1);
    printf("Writer %ld is writing.\n", (unsigned long)pthread_self());

    //simulate write
    sleep(1);

    //finish writing
    clock_t end = clock();
    new_signal(&resource_sem1);
    printf("Writer %ld has finished writing.\n", (unsigned long)pthread_self());

    double writer_tat = (double)(end - start) / CLOCKS_PER_SEC;
    writer_total += writer_tat;
    both_total += writer_tat;
    writer_count++;
    both_count++;

    return NULL;
}

int run_sol_one(int num_writers) {
    pthread_t reader_threads[10], writer_threads[num_writers];
    int reader_ids[10], writer_ids[num_writers];

    //initialize semaphores
    make_sem(&mutexSem, 1);
    make_sem(&resource_sem1, 1);

    //create 10 reader threads
    for (int i = 0; i < 10; i++) {
        reader_ids[i] = i + 1;  //assign unique id to readers
        pthread_create(&reader_threads[i], NULL, reader1, &reader_ids[i]);
    }

    // create n writer threads
    for (int i = 0; i < num_writers; i++) {
        writer_ids[i] = i + 1;  //assign unique id to writer
        pthread_create(&writer_threads[i], NULL, writer1, &writer_ids[i]);
    }

    //wait for all reader threads to complete
    for (int i = 0; i < 10; i++) {
        pthread_join(reader_threads[i], NULL);
    }

    //wait for all writer threads to complete
    for (int i = 0; i < num_writers; i++) {
        pthread_join(writer_threads[i], NULL);
    }

    // destroy semaphores
    destroy_sem(&mutexSem);
    destroy_sem(&resource_sem1);

    if (reader_count > 0) {
        double avg_reader = reader_total / reader_count;
        return avg_reader;
    }
    return 0;
}
