#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>


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
 
public synchronized void wait(Semaphore sem) {
    pthread_mutex_lock(sem.mutex); //lock thread
    while (sem.count <= 0){  //check if count <= 0, if so wait
        enqueue(&queue, pthread.id);
        pthread_cond_wait(sem.cond, sem.mutex);
    }
    sem.count--;                   // once count increases, decrement and unlock
    pthread_mutex_unlock(sem.mutex);
}

public synchronized void signal(Semaphore sem) {
    pthread_mutex_lock(sem.mutex); //lock thread
    sem.count++; //increment count
    if (!is_empty(&queue)) { //check if queue is not empty
        dequeue(&queue) //if not, dequeue element from queue and signal
        pthread_cond_signal(sem.condition)
    }
    pthread_mutex_unlock(sem.mutex); //unlock thread once finished
}

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

void initialize_queue(struct Queue *queue) {
    queue->front = NULL;
    queue->rear = NULL;
}

bool is_empty(struct Queue *queue) {
    if (queue->front == NULL) {
        return true;
    }
    return false;
}

void make_sem(struct Semaphore *semaphore, int starting_value) {
    semaphore->value = starting_value;
    pthread_mutex_init(&semaphore->mutex, NULL);
    pthread_cond_init(&semaphore->condition, NULL);
    initialize_queue(&semaphore->queue);
}

void destroy_sem(struct Semaphore *semaphore) {
    pthread_mutex_destroy(&semaphore->mutex);
    pthread_cond_destroy(&semaphore->condition);
}



int main() {
    struct Semaphore semaphore;
    make_sem(&semaphore, 1);

    destroy_sem(&semaphore);
    return 0;
}