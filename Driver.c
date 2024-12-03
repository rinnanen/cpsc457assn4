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

    // measure solution 1

    for (int i = 0; i < 10; i++) {
        run_sol_one(i);
    }

    // measure solution 2

    run_sol_two();


    // measure solution 3

    run_sol_three();

    return 0;
}