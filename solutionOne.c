#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include "Semaphore.h"




int readersCount;
struct Semaphore resource_sem1;
struct Semaphore mutexSem;

void *reader1(void *arg) {
    int thread_id = *((int *)arg);
    printf("Reader %ld is waiting.\n", pthread_self());
    new_wait(&mutexSem); //lock mutex and increase reader count
    readersCount++;
    if (readersCount == 1){
        new_wait(&resource_sem1); //if the first reader, lock resource as well
    }
    new_signal(&mutexSem); //unlock mutex for other readers to use
    printf("Reader %ld is reading.\n", pthread_self());

    //reading the resource

    new_wait(&mutexSem); //lock mutex to decrease reader count
    readersCount--;
    if (readersCount == 0) { //last reader unlocks resource
        new_signal(&resource_sem1);
    }
    new_signal(&mutexSem); //unlock mutex
    printf("Reader %ld has finished reading.\n", pthread_self());
    return NULL;
}

void *writer1(void *arg) {
    int thread_id = *((int *)arg);
    //wait to write
    printf("Writer %ld is waiting.\n", pthread_self());
    new_wait(&resource_sem1);
    printf("Writer %ld is writing.\n", pthread_self());

    //simulate write

    //finish writing
    new_signal(&resource_sem1);
    printf("Writer %ld has finished writing.\n", pthread_self());
    return NULL;
}

// int main(){
//     int randomVal = (rand() % (10 - 0 + 1)) + 0;
//     pthread_t reader_threads[10], writer_threads[randomVal];
//     int reader_ids[10], writer_ids[randomVal];

//     //initialize semaphores
//     make_sem(&mutexSem, 1);
//     make_sem(&resource_sem1, 1);

//     //create 10 reader threads
//     for (int i = 0; i < 10; i++) {
//         reader_ids[i] = i + 1;  //assign unique id to readers
//         pthread_create(&reader_threads[i], NULL, reader, &reader_ids[i]);
//     }

//     // create n writer threads
//     for (int i = 0; i < randomVal; i++) {
//         writer_ids[i] = i + 1;  //assign unique id to writer
//         pthread_create(&writer_threads[i], NULL, writer, &writer_ids[i]);
//     }

//     //wait for all reader threads to complete
//     for (int i = 0; i < 10; i++) {
//         pthread_join(reader_threads[i], NULL);
//     }

//     //wait for all writer threads to complete
//     for (int i = 0; i < randomVal; i++) {
//         pthread_join(writer_threads[i], NULL);
//     }

//     // destroy semaphores
//     semaphore_destroy(&mutexSem);
//     semaphore_destroy(&resource_sem1);
//     return 0;
// }
//solution1
// • Uses an integer variable readers
// o Shared between readers
// o Counts the number of readers accessing or trying to access the shared
// resource
// o Initially 0

// • Uses two semaphores:
// • resource: to synchronize access to the shared
// resource between readers and writers

// • mutex: to avoid race condition with the readers
// variable between readers

// • First reader locks resource, if available
// o Locking out writers
// • Subsequent readers do not need to lock it
// • Several readers can read simultaneously
// • A writer needs to lock the resource before
// accessing it
// o Locking both reader and writes
// • A stream of readers can indefinitely lock writers
// out

// Writer 
// <Entry Section>
// resource.wait()

// <Exit Section>
// resource.signal()

// Reader
// <Entry Section>
// mutex.wait()
// readers++
// if (readers == 1)
// resource.wait()
// mutex.signal()

// <Exit Section>
// mutex.wait()
// readers--
// If (readers == 0)
// resource.signal()
// mutex.signal()