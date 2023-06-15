#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include <stddef.h>
#include <errno.h>
#include <math.h>
#include <string.h>
#include "approxArsinh_lookup.h"
#include "approxArsinh_series.h"
#include <time.h>

static const long int numberOfImplementations = 4;     //number of possible Implementations to choose from
static const long int maxNumberOfRepetitions = 1000;   //max number of repetitions of the function call to avoid timeout
static const size_t iterations = 30;                   //iterations of series calculations

void help(void) {                            //prints options and use cases of the program
    printf("When parsing a negative number, make sure to use a double dash beforhand. \nElse it will be interpreted as a flags.\n");
}

double approxArsinh_series(double x);

double approxArsinh_lookup(double x);

double approxArsinh_predefined(double x) {
    return log(x + sqrt(x * x + 1));
}

double approxArsinh_series2(double x) {
    double e = x;
    double prev = x;
    double minusSqrt = -x * x;
    for (size_t k = 1; k < iterations; k++) {
        size_t num = (2 * k - 1) * (2 * k - 1);
        size_t den = (2 * k) * (2 * k + 1);
        prev = prev * (double) num * minusSqrt / (double) den;
        e += prev;
    }
    return e;
}
double relative_error(double x, int implementation){
    double exact = approxArsinh_predefined(x);
    double approx = 0.0;
    switch(implementation){
        case 0:
            approx = approxArsinh_series(x);
            break;
        case 1:
            approx = approxArsinh_lookup(x);
            break;
        default:
            printf("Error: Implementation not found!\n");
            return -1;
            break;
    }
    return fabs((approx-exact)/exact);
}

double performance(unsigned int n, double x, int implentation) {
    /*
    n = number of repititions (should be at least 20)
    x = input value
    implementation = 1: series ; 0: lookup table
    */
    struct timespec start, end;
    int c;
    switch (implentation)
    {
    case 1:
        c = clock_gettime(CLOCK_MONOTONIC, &start);
        if(c == -1){
            printf("Error: clock_gettime failed!\n");
            return EXIT_FAILURE;
        }
        for(unsigned int i = 0; i<n; i++){
            approxArsinh_series(x);
        }
        c = clock_gettime(CLOCK_MONOTONIC, &end);
        if(c == -1){
            printf("Error: clock_gettime failed!\n");
            return EXIT_FAILURE;
        }
        break;
    case 0:
        c = clock_gettime(CLOCK_MONOTONIC, &start);
        if(c == -1){
            printf("Error: clock_gettime failed!\n");
            return EXIT_FAILURE;
        }
        for(unsigned int i = 0; i<n; i++){
            approxArsinh_lookup(x);
        }
        c = clock_gettime(CLOCK_MONOTONIC, &end);
        if(c == -1){
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


int main(int argc, char *argv[]) {

    long int implementation = 0;          //choose Implementation
    long int runtime = 0;                 //if B-flag this sets numbers of executions for runtime measurement
    double number = 0.;                 //x

    errno = 0;
    char *endptr;
    int opt;

    while ((opt = getopt(argc, argv, "V:B:h:-:")) != -1) {
        switch (opt) {
            case 'V':
                implementation = strtol(optarg, &endptr, 10);
                if (endptr == optarg || *endptr != '\0') {
                    fprintf(stderr, "%s could not be converted to long.\nArgument may only contain digits 0-9\n",
                            optarg);
                    return EXIT_FAILURE;
                } else if (errno == ERANGE) {
                    fprintf(stderr,
                            "%s over- or underflows long. \nTo choose the desired implementation, select a number between 0 and %li.\n",
                            optarg, numberOfImplementations - 1);
                    return EXIT_FAILURE;
                }
                break;
            case 'B':
                runtime = strtol(optarg, &endptr, 10);
                if (endptr == optarg || *endptr != '\0') {
                    fprintf(stderr, "%s could not be converted to long.\nArgument may only contain digits 0-9\n",
                            optarg);
                    return EXIT_FAILURE;
                } else if (errno == ERANGE) {
                    fprintf(stderr,
                            "%s over- or underflows long. \nTo choose the iterations of the function, select a number between 1 and %li.\n",
                            optarg, maxNumberOfRepetitions);
                    return EXIT_FAILURE;
                }
                break;
            case 'h'  :
                help();
                return EXIT_SUCCESS;
            case '-'  :
                if (strcmp(optarg, "help") == 0) {
                    help();
                    return EXIT_SUCCESS;
                } else {
                    fprintf(stderr, "Unknown Option --\n");
                    return EXIT_FAILURE;
                }
            case '?':
                if (optopt == 'V' || optopt == 'B')
                    fprintf(stderr, "Options -V and -B require an Argument\n");
                else
                    fprintf(stderr,
                            "Unknown Option -%c\nWhen parsing a negative number, make sure to use a double dash beforhand. \nElse it will be interpreted as a flag.\n",
                            optopt);
                return EXIT_FAILURE;
                break;
            default:
                fprintf(stderr, "why tho   %c\n", optopt);
                return EXIT_FAILURE;
        }
    }

    if (optind != argc) {
        number = strtod(argv[optind], &endptr);
        if (endptr == argv[optind] || *endptr != '\0') {
            fprintf(stderr, "%s could not be converted to double. \n", argv[optind]);
            printf("Double may only contain the digits 0-9 and '.'.\n");
            printf("When multiplying with a power of 10, append 'e', followed by a positive or negative whole number as the exponent\n");
            return EXIT_FAILURE;
        } else if (errno == ERANGE) {
            fprintf(stderr, "%s over- or underflows double \n", argv[optind]);
            printf("datatype double has to be in range +/- 1.7E +/-308.\n");
            return EXIT_FAILURE;
        }
    }
    if (implementation < 0 || implementation >= numberOfImplementations) {
        fprintf(stderr, "%li is not a valid index for an implementation.\nChoose a value between 0 and %li.\n",
                implementation, numberOfImplementations - 1);
        return EXIT_FAILURE;
    }
    if (runtime < 0 || runtime > maxNumberOfRepetitions) {
        fprintf(stderr,
                "%li is not a valid number of repetitions of the function call.\nChoose a value between 1 and %li.\n",
                runtime, maxNumberOfRepetitions);
        return EXIT_FAILURE;
    }

    double result;
    if (runtime == 0) {
        switch (implementation) {
            case 0:
                result = approxArsinh_lookup(number);
                printf("Calculating arsinh(%f) with a lookup-table results in %f.\n", number, result);
                return EXIT_SUCCESS;
            case 1:
                result = approxArsinh_series(number);
                printf("Calculating arsinh(%f) with a series results in %f.\n", number, result);
                return EXIT_SUCCESS;
            case 2:
                result = approxArsinh_predefined(number);
                printf("Calculating arsinh(%f) with predefined functions of C results in %f.\n", number, result);
                return EXIT_SUCCESS;
            case 3:
                if (number > 1. || number < -1.) {
                    fprintf(stderr, "Using this Implementation is not recommended for any numbers x with |x|>1.\n");
                    printf("Due to the numeric divergence problem, values outside this interval lead to very imprecise results.\n");
                    printf("For greater values of x, use a different implementation like the lookup-table!\n");
                    return EXIT_FAILURE;
                }
                result = approxArsinh_series2(number);
                printf("Calculating arsinh(%f) with series2 results in %f.\n", number, result);
                return EXIT_SUCCESS;
            default:
                fprintf(stderr, "No such implementation.\n");
                return EXIT_FAILURE;
        }
    } else {
        double time = performance(runtime, 0.5, implementation);
        printf("The accumilated runtime of %ld iterations of implementation number %ld was %f seconds.\n",runtime, implementation, time);
        //fprintf(stderr, "Runtime-measurement not yet implemented\n");
    }


    return 0;
}

