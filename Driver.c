#include <stdio.h>
#include <pthread.h>
#include <unistd.h>
#include <time.h>

// header files
#include "Semaphore.h"
#include "solutionOne.h"
#include "solutionTwo.h"
#include "solutionThree.h"

int main() {
    struct timespec start, end;

    // measure solution 1
    clock_gettime(CLOCK_MONOTONIC, &start);
    run_sol_one();
    clock_gettime(CLOCK_MONOTONIC, &end);
    int sol1_tat = calculate_time(start, end);

    // measure solution 2
    clock_gettime(CLOCK_MONOTONIC, &start);
    run_sol_two();
    clock_gettime(CLOCK_MONOTONIC, &end);
    int sol2_tat = calculate_time(start, end);

    // measure solution 3
    clock_gettime(CLOCK_MONOTONIC, &start);
    run_sol_three();
    clock_gettime(CLOCK_MONOTONIC, &end);
    int sol3_tat = calculate_time(start, end);

    return 0;
}