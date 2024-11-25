#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include "Semaphore.c"

int readers_count;
int writers_count;
struct Semaphore write_mutex_sem;
struct Semaphore read_mutex_sem;
struct Semaphore readtry_mutex_sem;
struct Semaphore resource_sem;
struct Semaphore rentry_sem;

void *writer(void *arg) {
    int thread_id = *((int *)arg);
    new_wait(&write_mutex_sem);
    writers_count++;
    if (writers_count == 1) {
        new_wait(&readtry_mutex_sem);
    }

    new_wait(&write_mutex_sem);
    new_wait(&resource_sem);

    // read the resource

    new_signal(&resource_sem);
    new_wait(&write_mutex_sem);
    writers_count--;
    if (writers_count == 0) {
        new_signal(&readtry_mutex_sem);
    }
    new_signal(&write_mutex_sem);
    return NULL;
}

void *reader(void *arg) {
    int thread_id = *((int *)arg);
    new_wait(&rentry_sem);
    new_wait(&readtry_mutex_sem);
    new_wait(&read_mutex_sem);
    readers_count++;
    if (readers_count == 1) {
        new_wait(&resource_sem);
    }
    new_signal(&read_mutex_sem);
    new_signal(&readtry_mutex_sem);
    new_signal(&rentry_sem);

    // read the resource

    new_wait(&read_mutex_sem);
    readers_count--;
    if (readers_count == 0) {
        new_signal(&resource_sem);
    }
    new_signal(&read_mutex_sem);
    return NULL;
}



// duplicated it from sol1
// void main(){
//     int randomVal = (rand() % (10 - 0 + 1)) + 0;
//     pthread_t reader_threads[10], writer_threads[randomVal];
//     int reader_ids[10], writer_ids[randomVal];

//     //initialize semaphores
//     make_sem(&write_mutex_sem, 1);
//     make_sem(&read_mutex_sem, 1);
//     make_sem(&readtry_mutex_sem, 1);
//     make_sem(&resource_sem, 1);
//     make_sem(&rentry_sem, 1);

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
//     semaphore_destroy(&write_mutex_sem, 1);
//     semaphore_destroy(&read_mutex_sem, 1);
//     semaphore_destroy(&readtry_mutex_sem, 1);
//     semaphore_destroy(&resource_sem, 1);
//     semaphore_destroy(&rentry_sem, 1);

// }


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