#include "performance.h"

/*
 * Measures and Computes the runtime of a given arsinh implementation for a specified number of iterations
 *
 * Arguments: n (number of executions), x (Argument of the function), implementation [0;3]
 *
 * Returns: the runtime in seconds for arsinh(x)
 */
double performance(unsigned int n, double x, int implementation) {
    struct timespec start, end;
    int c1, c2;
    switch (implementation) {
        case 0:
            c1 = clock_gettime(CLOCK_MONOTONIC, &start);
            for (unsigned int i = 0; i < n; i++) {
                approxArsinh_lookup(x);
            }
            c2 = clock_gettime(CLOCK_MONOTONIC, &end);
            if (c1 == -1) {
                printf("Error: clock_gettime failed!\n");
                return EXIT_FAILURE;
            }
            if (c2 == -1) {
                printf("Error: clock_gettime failed!\n");
                return EXIT_FAILURE;
            }
            break;
        case 1:
            c1 = clock_gettime(CLOCK_MONOTONIC, &start);
            for (unsigned int i = 0; i < n; i++) {
                approxArsinh_series(x);
            }
            c2 = clock_gettime(CLOCK_MONOTONIC, &end);

            if (c1 == -1) {
                printf("Error: clock_gettime failed!\n");
                return EXIT_FAILURE;
            }
            if (c2 == -1) {
                printf("Error: clock_gettime failed!\n");
                return EXIT_FAILURE;
            }
            break;
        case 2:
            c1 = clock_gettime(CLOCK_MONOTONIC, &start);
            for (unsigned int i = 0; i < n; i++) {
                approxArsinh_series_V2(x);
            }
            c2 = clock_gettime(CLOCK_MONOTONIC, &end);

            if (c1 == -1) {
                printf("Error: clock_gettime failed!\n");
                return EXIT_FAILURE;
            }
            if (c2 == -1) {
                printf("Error: clock_gettime failed!\n");
                return EXIT_FAILURE;
            }
            break;

        case 3:
            c1 = clock_gettime(CLOCK_MONOTONIC, &start);
            for (unsigned int i = 0; i < n; i++) {
                approxArsinh_complexInstructions(x);
            }
            c2 = clock_gettime(CLOCK_MONOTONIC, &end);
            if (c1 == -1) {
                printf("Error: clock_gettime failed!\n");
                return EXIT_FAILURE;
            }
            if (c2 == -1) {
                printf("Error: clock_gettime failed!\n");
                return EXIT_FAILURE;
            }
            break;

        default:
            printf("Error: Implementation not found!\n");
            break;
    }
    double time = (end.tv_sec - start.tv_sec) + (end.tv_nsec - start.tv_nsec) / 1000000000.0;
    return time;
}

