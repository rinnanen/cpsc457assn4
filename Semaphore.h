#ifndef SEMAPHORE_H
#define SEMAPHORE_H

#include <pthread.h>
#include <stdbool.h>


struct Semaphore
{
    pthread_mutex_t mutex;
    pthread_cond_t condition;
    int value;
    struct Queue *queue;
};

struct Queue
{
    struct Node *front;
    struct Node *rear;
};

struct Node
{
    pthread_t id;
    struct Node *next;
};

void enqueue(struct Queue *queue, pthread_t id);
pthread_t dequeue(struct Queue *queue);
void initialize_queue(struct Queue *queue);
bool is_empty(struct Queue *queue);
void new_wait(struct Semaphore *sem);
void new_signal(struct Semaphore *sem);
void make_sem(struct Semaphore *semaphore, int starting_value);
void destroy_sem(struct Semaphore *semaphore);

#endif // SEMAPHORE_H
