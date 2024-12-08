#ifndef SOLUTIONTWO_H
#define SOLUTIONTWO_H
#include "tat_results.h"

//functions and variables for solution
struct Semaphore write_mutex_sem;
struct Semaphore read_mutex_sem;
struct Semaphore readtry_mutex_sem;
struct Semaphore resource_sem2;
struct Semaphore rentry_sem;


void *reader2(void *arg);
void *writer2(void *arg);
struct tat_results run_sol_two(int num_writers);


#endif // SOLUTIONTWO.H