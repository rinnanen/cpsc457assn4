#ifndef SOLUTIONTHREE_H
#define SOLUTIONTHREE_H

//functions and variables for solution
struct Semaphore resource_sem3;
struct Semaphore mutex_sem;

void *reader3(void *arg);
void *writer3(void *arg);
int run_sol_three();


#endif // SOLUTIONTHREE_H