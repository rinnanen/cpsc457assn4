#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <Semaphore.c>

int readers_count;
int writers_count;
struct Semaphore write_mutex_sem;
struct Semaphore read_mutex_sem;
struct Semaphore readtry_mutex_sem;
struct Semaphore resource_sem;
struct Semaphore rentry_sem;

void writer() {
    wait(&write_mutex_sem);
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
}

void reader() {
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