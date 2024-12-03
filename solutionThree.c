#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "Semaphore.h"

//functions and variables for solution
int readers_count;
int writers_count;
struct Semaphore resource_sem3;
struct Semaphore mutex_sem;

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

void *reader3(void *arg) {
    clock_t start = clock();
    int thread_id = *((int *)arg);
    printf("Reader %d is attempting to read.\n", thread_id);

    new_wait(&mutex_sem);
    if (writers_count > 0 || readers_count == 0) {
        printf("Reader %d is waiting as a writer is active or no readers are present.\n", thread_id);
        new_signal(&mutex_sem);
        new_wait(&resource_sem3);
        new_wait(&mutex_sem);
    }
    readers_count++;
    printf("Reader %d is reading. Total readers: %d.\n", thread_id, readers_count);
    new_signal(&mutex_sem);

    // read the resource
    sleep(1);

    clock_t end = clock();
    new_wait(&mutex_sem);
    readers_count--;
    printf("Reader %d has finished reading. Remaining readers: %d.\n", thread_id, readers_count);
    if (readers_count == 0) {
        printf("Reader %d is releasing the resource as no readers are left.\n", thread_id);
        new_signal(&resource_sem3);
    }
    new_signal(&mutex_sem);

    double reader_tat = (double)(end - start) / CLOCKS_PER_SEC;
    reader_total += reader_tat;
    both_total += reader_tat;
    reader_count++;
    both_count++;

    return NULL;
}

//errors if name is only writer
void *writer3(void *arg) {
    clock_t start = clock();
    int thread_id = *((int *)arg); //should cause errors relating to pthread_create to stop
    printf("Writer %d is attempting to write.\n", thread_id);

    new_wait(&mutex_sem);
    writers_count++;
    printf("Writer %d is waiting to acquire the resource. Total writers: %d.\n", thread_id, writers_count);
    new_signal(&mutex_sem);
    new_wait(&resource_sem3);

    // read the resource
    printf("Writer %d is writing.\n", thread_id);
    sleep(1);

    clock_t end = clock();
    new_wait(&mutex_sem);
    writers_count--;
    printf("Writer %d has finished writing. Remaining writers: %d.\n", thread_id, writers_count);
    new_signal(&mutex_sem);
    new_signal(&resource_sem3);

    double writer_tat = (double)(end - start) / CLOCKS_PER_SEC;
    writer_total += writer_tat;
    both_total += writer_tat;
    writer_count++;
    both_count++;

    return NULL;
}

// duplicated it from sol1
tat_results run_sol_three(int num_writers) {
    pthread_t reader_threads[10], writer_threads[num_writers];
    int reader_ids[10], writer_ids[num_writers];

    //initialize semaphores
    make_sem(&resource_sem3, 1);
    make_sem(&mutex_sem, 1);

    //create 10 reader threads
    for (int i = 0; i < 10; i++) {
        reader_ids[i] = i + 1;  //assign unique id to readers
        pthread_create(&reader_threads[i], NULL, reader3, &reader_ids[i]);
    }

    // create n writer threads
    for (int i = 0; i < num_writers; i++) {
        writer_ids[i] = i + 1;  //assign unique id to writer
        pthread_create(&writer_threads[i], NULL, writer3, &writer_ids[i]);
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
    destroy_sem(&resource_sem3);
    destroy_sem(&mutex_sem);

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