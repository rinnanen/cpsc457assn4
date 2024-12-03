#include <stdio.h>
#include <pthread.h>
#include <unistd.h>
#include <time.h>

// header files
#include "Semaphore.h"
#include "solutionOne.h"
#include "solutionTwo.h"
#include "solutionThree.h"

double sol1_reader = 0;
double sol1_writer = 0;
double sol1_both = 0;

double sol2_reader = 0;
double sol2_writer = 0;
double sol2_both = 0;

double sol3_reader = 0;
double sol3_writer = 0
double sol3_both = 0;

int main() {

    // measure solution 1
    print
    for (int i = 0; i < 10; i++) {
        run_sol_one(i);

    }

    // measure solution 2
    for (int i = 0; i < 10; i++) {
        run_sol_two(i);
    }

    // measure solution 3
    for (int i = 0; i < 10; i++) {
        run_sol_three(i);
    }

    return 0;
}