#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <Semaphore.c>

int readersCount;
Semaphore resourceSem;
Semaphore mutexSem;

void reader(){
    wait(&mutexSem); //lock mutex and increase reader count
    readersCount++;
    if (readersCount == 1){
        wait(&resourceSem); //if the first reader, lock resource as well
    }
    signal(&mutexSem); //unlock mutex for other readers to use

    //reading the resource

    wait(&mutexSem); //lock mutex to decrease reader count
    readersCount--;
    if (readersCount == 0) { //last reader unlocks resource
        signal(&resource);
    }
    signal(&mutex); //unlock mutex
}

void writer(){
    //wait to write
    wait(&resource);

    //simulate write

    //finih writing
    signal(&resource);
}

void main(){
    int randomVal = (rand() % (10 - 0 + 1)) + 0;
    pthread_t reader_threads[10], writer_threads[randomVal];
    int reader_ids[10], writer_ids[randomVal];

    //initialize semaphores
    make_sem(&mutex, 1);
    make_sem(&resource, 1);

    //create 10 reader threads
    for (int i = 0; i < 10; i++) {
        reader_ids[i] = i + 1;  //assign unique Id to readers
        if (pthread_create(&reader_threads[i], NULL, reader, &reader_ids[i]) != 0) {
            perror("failed to create reader thread");
            exit(1);
        }
    }

    // create n writer threads
    for (int i = 0; i < randomVal; i++) {
        writer_ids[i] = i + 1;  // Assign unique IDs to writers
        if (pthread_create(&writer_threads[i], NULL, writer, &writer_ids[i]) != 0) {
            perror("Failed to create writer thread");
            exit(1);
        }
    }

    //wait for all reader threads to complete
    for (int i = 0; i < 10; i++) {
        pthread_join(reader_threads[i], NULL);
    }

    // Wait for all writer threads to complete
    for (int i = 0; i < randomVal; i++) {
        pthread_join(writer_threads[i], NULL);
    }

    // destroy semaphores
    semaphore_destroy(&mutex);
    semaphore_destroy(&resource);

}
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