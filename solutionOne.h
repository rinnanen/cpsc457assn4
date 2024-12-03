#ifndef SOLUTIONONE_H
#define SOLUTIONONE_H

#include <pthread.h>

//functions and variables for solution
int readersCount;
struct Semaphore resource_sem1;
struct Semaphore mutexSem;

struct tat_results;

void *reader1(void *arg);
void *writer1(void *arg);
struct tat_results run_sol_one(int num_writers);

#endif //SOLUTIONONE.H