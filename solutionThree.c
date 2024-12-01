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


void *reader3(void *arg) {
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

    new_wait(&mutex_sem);
    readers_count--;
    printf("Reader %d has finished reading. Remaining readers: %d.\n", thread_id, readers_count);
    if (readers_count == 0) {
        printf("Reader %d is releasing the resource as no readers are left.\n", thread_id);
        new_signal(&resource_sem3);
    }
    new_signal(&mutex_sem);
    return NULL;
}

//errors if name is only writer
void *writer3(void *arg) {
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

    new_wait(&mutex_sem);
    writers_count--;
    printf("Writer %d has finished writing. Remaining writers: %d.\n", thread_id, writers_count);
    new_signal(&mutex_sem);
    new_signal(&resource_sem3);
    return NULL;
}

// duplicated it from sol1
int run_sol_three(){
    int randomVal = (rand() % (10 - 0 + 1)) + 0;
    pthread_t reader_threads[10], writer_threads[randomVal];
    int reader_ids[10], writer_ids[randomVal];

    //initialize semaphores
    make_sem(&resource_sem3, 1);
    make_sem(&mutex_sem, 1);

    //create 10 reader threads
    for (int i = 0; i < 10; i++) {
        reader_ids[i] = i + 1;  //assign unique id to readers
        pthread_create(&reader_threads[i], NULL, reader3, &reader_ids[i]);
    }

    // create n writer threads
    for (int i = 0; i < randomVal; i++) {
        writer_ids[i] = i + 1;  //assign unique id to writer
        pthread_create(&writer_threads[i], NULL, writer3, &writer_ids[i]);
    }

    //wait for all reader threads to complete
    for (int i = 0; i < 10; i++) {
        pthread_join(reader_threads[i], NULL);
    }

    //wait for all writer threads to complete
    for (int i = 0; i < randomVal; i++) {
        pthread_join(writer_threads[i], NULL);
    }

    // destroy semaphores
    destroy_sem(&resource_sem3);
    destroy_sem(&mutex_sem);
    return 0;
}

int main() {
    run_sol_three();
}

//solution3

// Uses two shared variables:
// • readers: counts the number of trying readers

// • writers: counts the number of trying writers

// • Uses two shared semaphores:
// • mutex: synchronizes access to readers and
// writers
// • resource: controls access to CS

// Writer 
// <Entry Section>
// mutex.wait()
// writers++
// mutex.signal()
// resource.wait()

// <Exit Section>
// mutex.wait()
// writers--
// mutex.signal()
// resource.signal()

// Reader
// <Entry Section>
// mutex.wait()
// if (writers > 0 or readers ==0) {
// mutex.signal()
// resource.wait()
// mutex.wait()
// }
// readers++
// mutex.signal()

// <Exit Section>
// mutex.wait()
// readers--
// If (readers == 0) resource.signal()
// mutex.signal()