#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include "Semaphore.h"


//functions and variables for solution
int readerCount;
int writerCount;
struct Semaphore write_mutex_sem;
struct Semaphore read_mutex_sem;
struct Semaphore readtry_mutex_sem;
struct Semaphore resource_sem2;
struct Semaphore rentry_sem;

double reader_total = 0;
int reader_count = 0;

double writer_total = 0;
int writer_count = 0;

double both_total = 0;
int both_count = 0;

typedef struct {
    double reader_tat;
    double writer_tat;
    double both_tat;
} tat_results;

//errors if name is only reader
void *reader2(void *arg) {
    clock_t start = clock();
    int thread_id = *((int *)arg);

    printf("Reader %ld is waiting to enter.\n", (unsigned long)pthread_self());
    new_wait(&rentry_sem);
    new_wait(&readtry_mutex_sem);
    new_wait(&read_mutex_sem);
    readerCount++;
    if (readerCount == 1) {
        printf("Reader %ld is locking the resource for readers.\n", (unsigned long)pthread_self());
        new_wait(&resource_sem2);
    }
    new_signal(&read_mutex_sem);
    new_signal(&readtry_mutex_sem);
    new_signal(&rentry_sem);

    printf("Reader %ld is reading.\n", (unsigned long)pthread_self());

    sleep(1);
    // read the resource

    clock_t end = clock();
    new_wait(&read_mutex_sem);
    readerCount--;
    if (readerCount == 0) {
        printf("Reader %ld is unlocking the resource for writers.\n", (unsigned long)pthread_self());
        new_signal(&resource_sem2);
    }
    new_signal(&read_mutex_sem);
    printf("Reader %ld has finished reading.\n", (unsigned long)pthread_self());

    double reader_tat = (double)(end - start) / CLOCKS_PER_SEC;
    reader_total += reader_tat;
    both_total += reader_tat;
    reader_count++;
    both_count++;

    return NULL;
}

//errors if name is only writer
void *writer2(void *arg) {
    clock_t start = clock();
    int thread_id = *((int *)arg);

    printf("Writer %ld is waiting to enter.\n", (unsigned long)pthread_self());
    new_wait(&write_mutex_sem);
    printf("Writer %ld has acquired write mutex.\n", (unsigned long)pthread_self());
    writerCount++;
    if (writerCount == 1) {
        printf("Writer %ld is locking the read-try mutex.\n", (unsigned long)pthread_self());
        new_wait(&readtry_mutex_sem);
    }

    new_signal(&write_mutex_sem);
    printf("Writer %ld is waiting for resource access.\n", (unsigned long)pthread_self());
    new_wait(&resource_sem2);

    // read the resource
    printf("Writer %ld is writing.\n", (unsigned long)pthread_self());

    sleep(1);

    clock_t end = clock();
    new_signal(&resource_sem2);
    printf("Writer %ld has finished writing and released the resource.\n", (unsigned long)pthread_self());
    new_wait(&write_mutex_sem);
    writerCount--;
    if (writerCount == 0) {
        printf("Writer %ld is unlocking the read-try mutex (last writer).\n", (unsigned long)pthread_self());
        new_signal(&readtry_mutex_sem);
    }
    new_signal(&write_mutex_sem);
    printf("Writer %ld has released the write mutex.\n", (unsigned long)pthread_self());
    
    double writer_tat = (double)(end - start) / CLOCKS_PER_SEC;
    writer_total += writer_tat;
    both_total += writer_tat;
    writer_count++;
    both_count++;

    return NULL;
}


// duplicated it from sol1
tat_results run_sol_two(int num_writers) {
    pthread_t reader_threads[10], writer_threads[num_writers];
    int reader_ids[10], writer_ids[num_writers];

    //initialize semaphores
    make_sem(&write_mutex_sem, 1);
    make_sem(&read_mutex_sem, 1);
    make_sem(&readtry_mutex_sem, 1);
    make_sem(&resource_sem2, 1);
    make_sem(&rentry_sem, 1);

    //create 10 reader threads
    for (int i = 0; i < 10; i++) {
        reader_ids[i] = i + 1;  //assign unique id to readers
        pthread_create(&reader_threads[i], NULL, reader2, &reader_ids[i]);
    }

    // create n writer threads
    for (int i = 0; i < num_writers; i++) {
        writer_ids[i] = i + 1;  //assign unique id to writer
        pthread_create(&writer_threads[i], NULL, writer2, &writer_ids[i]);
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
    destroy_sem(&write_mutex_sem);
    destroy_sem(&read_mutex_sem);
    destroy_sem(&readtry_mutex_sem);
    destroy_sem(&resource_sem2);
    destroy_sem(&rentry_sem);


    tat_results results;

    double avg_reader = 0;
    if (reader_count > 0) {
        double avg_reader = reader_total / reader_count;
    }
    results.reader_tat = avg_reader;

    double avg_writer = 0;
    if (writer_count > 0) {
        double avg_writer = writer_total / writer_count;
    }
    results.writer_tat = avg_writer;

    double avg_both = 0;
    if (both_count > 0) {
        double avg_both = both_total / both_count;
    }
    results.both_tat = avg_both;

    return results;
}