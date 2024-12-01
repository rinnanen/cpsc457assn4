#ifndef SOLUTIONONE_H
#define SOLUTIONONE_H

#include <pthread.h>

//functions and variables for solution
int readersCount;
struct Semaphore resource_sem1;
struct Semaphore mutexSem;

void *reader1(void *arg);
void *writer1(void *arg);
int run_sol_one();

#endif //SOLUTIONONE.H