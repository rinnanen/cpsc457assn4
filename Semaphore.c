#include <pthread.h>
#include <stdio.h>

struct Semaphore {
    pthread_mutex_t mutex;
    pthread_cond_t condition;
    int value;
};

struct Queue {
    struct Node* front;
    struct Node* rear;
};

struct Node {
    pthread_t id;
    struct Node* next;
};