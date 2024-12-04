#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "Semaphore.h"

//functions and variables for solution
int readers_count3;
int writers_count3;
struct Semaphore resource_sem3;
struct Semaphore mutex_sem;

double reader_total3 = 0;
int reader_count3 = 0;

double writer_total3 = 0;
int writer_count3 = 0;

double both_total3 = 0;
int both_count3 = 0;

typedef struct {
    double reader_tat3;
    double writer_tat3;
    double both_tat3;
} tat_results;

void *reader3(void *arg) {
    clock_t start = clock();
    int thread_id = *((int *)arg);

    new_wait(&mutex_sem);
    if (writers_count3 > 0 || readers_count3 == 0) {
        new_signal(&mutex_sem);
        new_wait(&resource_sem3);
        new_wait(&mutex_sem);
    }
    readers_count3++;
    new_signal(&mutex_sem);

    // read the resource
    usleep(10 * 1000);

    new_wait(&mutex_sem);
    readers_count3--;
    if (readers_count3 == 0) {
        new_signal(&resource_sem3);
    }
    new_signal(&mutex_sem);
    clock_t end = clock();

    double reader_tat = (double)(end - start);
    reader_total3 += reader_tat;
    both_total3 += reader_tat;
    reader_count3++;
    both_count3++;

    return NULL;
}

//errors if name is only writer
void *writer3(void *arg) {
    clock_t start = clock();
    int thread_id = *((int *)arg); //should cause errors relating to pthread_create to stop

    new_wait(&mutex_sem);
    writers_count3++;
    new_signal(&mutex_sem);
    new_wait(&resource_sem3);

    // read the resource
    usleep(10 * 1000);

    new_wait(&mutex_sem);
    writers_count3--;
    new_signal(&mutex_sem);
    new_signal(&resource_sem3);
    clock_t end = clock();

    double writer_tat = (double)(end - start);
    writer_total3 += writer_tat;
    both_total3 += writer_tat;
    writer_count3++;
    both_count3++;

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

    tat_results results3;


    double avg_reader3 = 0;
    if (reader_count3 > 0) {
        avg_reader3 = reader_total3 / reader_count3;
        results3.reader_tat3 = avg_reader3;
    }
    results3.reader_tat3 = avg_reader3;

    double avg_writer3 = 0;
    if (writer_count3 > 0) {
        avg_writer3 = writer_total3 / writer_count3;
    }
    results3.writer_tat3 = avg_writer3;

    double avg_both3 = 0;
    if (both_count3 > 0) {
        avg_both3 = both_total3 / both_count3;
    }
    results3.both_tat3 = avg_both3;

    reader_total3 = 0;
    reader_count3 = 0;

    return results3;
}