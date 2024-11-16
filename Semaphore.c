#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

struct Semaphore {
    pthread_mutex_t mutex;
    pthread_cond_t condition;
    int value;
    struct Queue queue;
};

struct Queue {
    struct Node* front;
    struct Node* rear;
};

struct Node {
    pthread_t id;
    struct Node* next;
};

void enqueue(struct Queue *queue, pthread_t id) {
    struct Node* new_node = (struct Node*)malloc(sizeof(struct Node));
    new_node->id = id;
    new_node->next = NULL;

    if (queue->rear != NULL) {
        queue->rear->next = new_node;
    } else {
        queue->front = new_node;
    }

    queue->rear = new_node;
}

pthread_t dequeue(struct Queue *queue) {
    if (queue->front == NULL) {
        return 0;
    }

    struct Node* temp = queue->front;

    pthread_t id = temp->id;

    queue->front = queue->front->next;

    if (queue->front == NULL) {
        queue->rear = NULL;
    }

    free(temp);
    return id;
}

bool is_empty(struct Queue *queue) {
    if (queue->front == NULL) {
        return true;
    }
    return false;
}