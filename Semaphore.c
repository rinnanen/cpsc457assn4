#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "Semaphore.h"

//custom enqueue function for Semaphores
void enqueue(struct Queue *queue, pthread_t id)
{
    struct Node *new_node = (struct Node *)malloc(sizeof(struct Node));
    new_node->id = id;
    new_node->next = NULL;

    if (queue->rear != NULL)
    {
        queue->rear->next = new_node;
    }
    else
    {
        queue->front = new_node;
    }

    queue->rear = new_node;
}

//custom dequeue function for Semaphores
pthread_t dequeue(struct Queue *queue)
{
    if (queue->front == NULL)
    {
        return 0;
    }

    struct Node *temp = queue->front;

    pthread_t id = temp->id;

    queue->front = queue->front->next;

    if (queue->front == NULL)
    {
        queue->rear = NULL;
    }

    free(temp);
    return id;
}

//start queue
void initialize_queue(struct Queue *queue)
{
    queue->front = NULL;
    queue->rear = NULL;
}

//check if is empty
bool is_empty(struct Queue *queue)
{
    if (queue->front == NULL)
    {
        return true;
    }
    return false;
}

//custom wait function 
void new_wait(struct Semaphore *sem) {
    pthread_mutex_lock(&sem->mutex); // lock thread
    while (sem->value <= 0) { // check if count <= 0, if so wait
        enqueue(sem->queue, pthread_self());
        pthread_cond_wait(&sem->condition, &sem->mutex);
        dequeue(sem->queue);
    }
    sem->value--; // once count increases, decrement and unlock
    pthread_mutex_unlock(&sem->mutex);
}

//custom wait function 
void new_signal(struct Semaphore *sem){
    pthread_mutex_lock(&sem->mutex); // lock thread
    sem->value++;                    // increment count
    if (!is_empty(sem->queue)) {   // check if queue is not empty
        pthread_cond_signal(&sem->condition);
    }
    pthread_mutex_unlock(&sem->mutex); // unlock thread once finished
}

//make a new semaphore
void make_sem(struct Semaphore *semaphore, int starting_value){
    semaphore->value = starting_value;
    pthread_mutex_init(&semaphore->mutex, NULL);
    pthread_cond_init(&semaphore->condition, NULL);

    semaphore->queue = (struct Queue *)malloc(sizeof(struct Queue));
    initialize_queue(semaphore->queue);
}

//destroy existing semaphore
void destroy_sem(struct Semaphore *semaphore){

    free(semaphore->queue);

    pthread_mutex_destroy(&semaphore->mutex);
    pthread_cond_destroy(&semaphore->condition);
}
