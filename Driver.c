#include <stdio.h>
#include <pthread.h>
#include <unistd.h>
#include <time.h>

// header files
#include "Semaphore.h"
#include "solutionOne.h"
#include "solutionTwo.h"
#include "solutionThree.h"
#include "tat_results.h"

double sol1_reader = 0;
double sol1_writer = 0;
double sol1_both = 0;

double sol2_reader = 0;
double sol2_writer = 0;
double sol2_both = 0;

double sol3_reader = 0;
double sol3_writer = 0;
double sol3_both = 0;

int main() {

    // measure solution 1
    printf("Readers-Writers Solution 1 (time in seconds)\n");
    printf("%-8s %-16s %-16s %-16s\n", "Writers", "AVG reader TAT", "AVG writer TAT", "AVGTAT");
    fflush(stdout);

    for (int i = 0; i < 11; i++) {
        struct tat_results sol_one = run_sol_one(i);
        double rtat = sol_one.reader_tat;
        double wtat = sol_one.writer_tat;
        double btat = sol_one.general_tat;

        printf("%-8d %-16.2f %-16.2f %-16.2f\n", i, rtat, wtat, btat);
        fflush(stdout);
    }

    // measure solution 2
    printf("\nReaders-Writers Solution 2 (time in seconds)\n");
    printf("%-8s %-16s %-16s %-16s\n", "Writers", "AVG reader TAT", "AVG writer TAT", "AVGTAT");
    fflush(stdout);

    for (int i = 0; i < 11; i++) {
        struct tat_results sol_two = run_sol_two(i);
        double rtat2 = sol_two.reader_tat / 1000;
        double wtat2 = sol_two.writer_tat / 1000;
        double btat2 = sol_two.general_tat / 1000;

        printf("%-8d %-16.2f %-16.2f %-16.2f\n", i, rtat2, wtat2, btat2);
        fflush(stdout);
    }

    // measure solution 3
    printf("\nReaders-Writers Solution 3 (time in seconds)\n");
    printf("%-8s %-16s %-16s %-16s\n", "Writers", "AVG reader TAT", "AVG writer TAT", "AVGTAT");
    fflush(stdout);

    for (int i = 0; i < 11; i++) {
        struct tat_results sol_three = run_sol_three(i);
        double rtat3 = sol_three.reader_tat / 1000;
        double wtat3 = sol_three.writer_tat / 1000;
        double btat3 = sol_three.general_tat / 1000;

        printf("%-8d %-16.2f %-16.2f %-16.2f\n", i, rtat3, wtat3, btat3);
        fflush(stdout);
    }

    return 0;
}