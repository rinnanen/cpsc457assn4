#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include "Semaphore.h"


//functions and variables for solution
int readerCount;
int writerCount;
struct Semaphore write_mutex_sem;
struct Semaphore read_mutex_sem;
struct Semaphore readtry_mutex_sem;
struct Semaphore resource_sem2;
struct Semaphore rentry_sem;


//errors if name is only reader
void *reader2(void *arg) {
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

    new_wait(&read_mutex_sem);
    readerCount--;
    if (readerCount == 0) {
        printf("Reader %ld is unlocking the resource for writers.\n", (unsigned long)pthread_self());
        new_signal(&resource_sem2);
    }
    new_signal(&read_mutex_sem);
    printf("Reader %ld has finished reading.\n", (unsigned long)pthread_self());
    return NULL;
}

//errors if name is only writer
void *writer2(void *arg) {
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
    return NULL;
}


// duplicated it from sol1
int main(){
    int randomVal = (rand() % (10 - 0 + 1)) + 0;
    pthread_t reader_threads[10], writer_threads[randomVal];
    int reader_ids[10], writer_ids[randomVal];

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
    for (int i = 0; i < randomVal; i++) {
        writer_ids[i] = i + 1;  //assign unique id to writer
        pthread_create(&writer_threads[i], NULL, writer2, &writer_ids[i]);
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
    destroy_sem(&write_mutex_sem);
    destroy_sem(&read_mutex_sem);
    destroy_sem(&readtry_mutex_sem);
    destroy_sem(&resource_sem2);
    destroy_sem(&rentry_sem);

}


//solution2
// Uses variables
// • writers: number of writers in the game
// o Shared by writers
// o Initially 0

// • readers: number of readers in the game
// o Shared by readers
// o Initially 0

// • Uses 5 semaphores!
// • 2 shared by readers and writers:
// • resource: controls access to the resource
// • readtry: allows writers to “bully” readers from trying

// • 1 semaphore shared by writers
// • wmutex: avoids race conditions on variable writers

// • 2 semaphores shared by readers
// • rmutex: avoids race conditions on variable readers
// • rentry: needed to favor writers

// Writer 
// <Entry Section>
// wmutex.wait()
// writers++
// if (writers == 1) readtry.wait()
// wmutex.signal()
// resource.wait()

// <Exit Section>
// resource.signal()
// wmutex.wait()
// writers--
// if (writers == 0)
// readtry.signal()
// wmutex.signal()

// Reader
// <Entry Section>
// rentry.wait()
// readtry.wait()
// rmutex.wait()
// readers++
// if (readers == 1) resource.wait()
// rmutex.signal()
// readtry.signal()
// rentry.signal()

// <Exit Section>
// rmutex.wait()
// readers--
// If (readers == 0) resource.signal()
// rmutex.signal()