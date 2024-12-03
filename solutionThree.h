#ifndef SOLUTIONTHREE_H
#define SOLUTIONTHREE_H
#include "tat_results.h"

//functions and variables for solution
struct Semaphore resource_sem3;
struct Semaphore mutex_sem;

void *reader3(void *arg);
void *writer3(void *arg);
struct tat_results run_sol_three(int num_writers);


#endif // SOLUTIONTHREE_H