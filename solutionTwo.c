#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include "Semaphore.h"


//functions and variables for solution
//based on assignment description
int readerCount2;
int writerCount2;
struct Semaphore write_mutex_sem;
struct Semaphore read_mutex_sem;
struct Semaphore readtry_mutex_sem;
struct Semaphore resource_sem2;
struct Semaphore rentry_sem;

double reader_total2 = 0;
int reader_count2 = 0;

double writer_total2 = 0;
int writer_count2 = 0;

double both_total2 = 0;
int both_count2 = 0;

typedef struct {
    double reader_tat2;
    double writer_tat2;
    double both_tat2;
} tat_results;

//errors if name is only reader
void *reader2(void *arg) {
    clock_t start = clock();
    int thread_id = *((int *)arg);
    //lock semaphores and increae reader count
    new_wait(&rentry_sem);
    new_wait(&readtry_mutex_sem);
    new_wait(&read_mutex_sem);
    readerCount2++;
    //if only 1 reader is waiting we lock the resource
    if (readerCount2 == 1) {
        new_wait(&resource_sem2);
    }
    //unlock for other readers to use
    new_signal(&read_mutex_sem);
    new_signal(&readtry_mutex_sem);
    new_signal(&rentry_sem);

    usleep(10 * 1000);
    // read the resource

    new_wait(&read_mutex_sem);//lock read mutex and decrease reader count
    readerCount2--;

    //once there are no more readers waiting we unlock the resource
    if (readerCount2 == 0) {
        new_signal(&resource_sem2);
    }
    new_signal(&read_mutex_sem);//unlock read mutex

    //tat calculations
    clock_t end = clock();

    double reader_tat = (double)(end - start);
    reader_total2 += reader_tat;
    both_total2 += reader_tat;
    reader_count2++;
    both_count2++;

    return NULL;
}

//errors if name is only writer
void *writer2(void *arg) {
    clock_t start = clock();
    int thread_id = *((int *)arg);

    //lock write mutex
    new_wait(&write_mutex_sem);
    //increase writer count
    writerCount2++;
    //check if only 1 writer is waiting 
    if (writerCount2 == 1) {
        new_wait(&readtry_mutex_sem);
    }

    //unlock write mutex
    new_signal(&write_mutex_sem);

    //lock the resource
    new_wait(&resource_sem2);

    // read the resource
    usleep(10 * 1000);

    //unlock the reource
    new_signal(&resource_sem2);

    //lock write mutex and decrease writer count
    new_wait(&write_mutex_sem);
    writerCount2--;

    //if no more writers waiting, unlock readtry sem
    if (writerCount2 == 0) {
        new_signal(&readtry_mutex_sem);
    }
    //unlock wrtie mutex
    new_signal(&write_mutex_sem);
    

    //tat calculations
    clock_t end = clock();

    double writer_tat2 = (double)(end - start);
    writer_total2 += writer_tat2;
    both_total2 += writer_tat2;
    writer_count2++;
    both_count2++;

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



    tat_results results2;

    double avg_reader2 = 0; // initialize in case of no readers
    if (reader_count2 > 0) {
        avg_reader2 = reader_total2 / reader_count2;
    }
    // assign it to 0 if there are no readers
    results2.reader_tat2 = avg_reader2;
    
    double avg_writer2 = 0;// initialize in case of no writers
    if (writer_count2 > 0) {
        avg_writer2 = writer_total2 / writer_count2;
    }
    // assign it to 0 if there are no writers
    results2.writer_tat2 = avg_writer2;

    double avg_both2 = 0;// initialize in case of no processes
    if (both_count2 > 0) {
        avg_both2 = both_total2 / both_count2;
    }
    // assign it to 0 if there are no processes
    results2.both_tat2 = avg_both2;

    return results2;
}