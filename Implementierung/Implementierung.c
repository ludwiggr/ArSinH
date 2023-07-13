#include <stdio.h>
#include <stdlib.h>
#include <getopt.h>
#include <stddef.h>
#include <errno.h>
#include <math.h>
#include <time.h>
#include <stdbool.h>
#include "approxArsinh_lookup.h"
#include "approxArsinh_series.h"
#include "approxArsinh_complexInstructions.h"

/* Non-changing global variables for testing */
static const long int numberOfImplementations = 4;
static const long int maxNumberOfRepetitions = 1000000000;
/* A useful help message to be printed in the frame program */
static const char *help_msg =
        "Options and Use Cases:\n"
        "The program calculates the arsinh(x) = ln(x + sqrt(x^2 + 1)) for an input value.\n"
        "\n"
        "Positional arguments:\n"
        "  x   The input value for which the arsinh is calculated\n"
        "      Can be any positive or negative double including +-inf and +-nan\n"
        "\n"
        "Optional arguments:\n"
        "  -V <X:Number>    The Implementation, that should be used to approximate the arsinh(x)(default: X = 0)\n"
        "                   Implementations corresponding to X:\n"
        "               0: Lookuptable with linear Interpolation\n"
        "               1: ('Pure') Taylor Series of the arsinh(x) around 0\n"
        "                  The convergence zone of the series is [-1; 1]\n"
        "               2: Approximation using different series depending on the input value\n"
        "                  provides meaningful results for all possible inputs\n"
        "               3: Approximation using the complex instructions sqrt and log\n"
        "  -B [X:Number]    if set, program measures runtime of X iterations of the calculation (default: X = 100000000)\n"
        "  -R               Calculates relative error of the result compared to the actual value\n"
        "  -h/--help        Show help message (this text) and exit\n";


double approxArsinh_lookup(double x);

double approxArsinh_series(double x);

double approxArsinh_differentSeries(double x);

double approxArsinh_complexInstructions(double x);

/* Multiplexes the Implementation to be used
 *
 * Returns: An Approximation of the arsinh(x) for the Argument x and the Argument implementation [0;3]
 */
double calculate_result(double x, int implementation) {  //just calculates the result and returns the double
    switch (implementation) {
        case 0:
            return approxArsinh_lookup(x);
        case 1:
            return approxArsinh_series(x);
        case 2:
            return approxArsinh_differentSeries(x);
        case 3:
            return approxArsinh_complexInstructions(x);
        default:
            fprintf(stderr, "No such implementation.\n");
            return EXIT_FAILURE;
    }
}

/* Computes the relative error of a given arsinh implementation compared to the math library
 *
 * Returns: the relative error for arsinh(x) for the arguments x and the Argument implementation [0;3]
 */
double relative_error(double x, int implementation) {
    double exact = asinh(x);
    double approx;
    switch (implementation) {
        case 0:
            approx = approxArsinh_lookup(x);
            break;
        case 1:
            approx = approxArsinh_series(x);
            break;
        case 2:
            approx = approxArsinh_differentSeries(x);
            break;
        case 3:
            approx = approxArsinh_complexInstructions(x);
            break;
        default:
            printf("Error: Implementation not found!\n");
            return -1;
    }
    return fabs((approx - exact) / exact);
}


/* Measures and Computes the performance of a given arsinh implementation compared to the math library
 *
 * Returns: the relative error for arsinh(x) for the Argument n (number of executions) the Arguments x (Argument of the function) and the Argument implementation [0;3]
 */
double performance(unsigned int n, double x,
                   int implementation) {
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
                approxArsinh_differentSeries(x);
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


/* Frame program that parses and interprets user input
 *
 * Returns: One of {EXIT_SUCCESS, EXIT_FAILURE}
 */
int main(int argc, char *argv[]) {

    long int implementation = 0;
    long int iterations = 0;
    double newNum;
    bool negativeNumber_Set = false;
    int negativeNumber_Index = 0;
    bool relativeError = false;
    double number;

    errno = 0;
    char *endptr;
    int opt;
    int option_index = 0;
    struct option long_options[] = {      //converts --help to -h
            {"help", no_argument, NULL, 'h'},
            {NULL,   0,           NULL, 0}
    };
    while ((opt = getopt_long(argc, argv, "V:B::hR0::1::2::3::4::5::6::7::8::9::.::i::I::n::N::", long_options,
                              &option_index)) != -1) {
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
                } else if (implementation < 0 || implementation >= numberOfImplementations) {
                    fprintf(stderr,
                            "%li is not a valid index for an implementation.\nChoose a value between 0 and %li.\n",
                            implementation, numberOfImplementations - 1);
                    return EXIT_FAILURE;
                }
                break;
            case 'B':
                if (optarg == NULL && optind < argc && argv[optind][0] != '-') {
                    optarg = argv[optind++];
                }
                if (optarg != NULL) {
                    iterations = strtol(optarg, &endptr, 10);
                    if (endptr == optarg || *endptr != '\0') {
                        fprintf(stderr, "%s could not be converted to long.\nArgument may only contain digits 0-9.\n",
                                optarg);
                        printf("If you wanted to parse the input value for the arsinh function, please don't parse it directly after the -B flag.\n");
                        return EXIT_FAILURE;
                    } else if (errno == ERANGE) {
                        fprintf(stderr,
                                "%s over- or underflows long. \nTo choose the iterations of the function, select a number between 1 and %li.\n",
                                optarg, maxNumberOfRepetitions);
                        printf("If you wanted to parse the input value for the arsinh function, please don't parse it directly after the -B flag.\n");

                        return EXIT_FAILURE;
                    } else if (iterations < 0 || iterations > maxNumberOfRepetitions) {
                        fprintf(stderr,
                                "%li is not a valid number of repetitions of the function call.\nChoose a value between 1 and %li.\n",
                                iterations, maxNumberOfRepetitions);
                        printf("If you wanted to parse the input value for the arsinh function, please don't parse it directly after the -B flag.\n");
                        return EXIT_FAILURE;
                    }
                } else {
                    iterations = 100000000;
                }
                break;
            case 'R'  :
                relativeError = true;
                break;
            case 'h'  :
                printf("%s", help_msg);
                return EXIT_SUCCESS;

                //if the - is followed by one of these chars, we assume that
                //the user might want to parse a negative number as a positional argument
            case '0'  :
            case '1'  :
            case '2'  :
            case '3'  :
            case '4'  :
            case '5'  :
            case '6'  :
            case '7'  :
            case '8'  :
            case '9'  :
            case '.'  :
            case 'i'  :
            case 'n'  :
            case 'I'  :
            case 'N'  :
                newNum = strtod(argv[optind - 1], &endptr);
                if (endptr == argv[optind - 1] || *endptr != '\0') {
                    fprintf(stderr, "%c is not a valid Option and %s could not be converted to double\n", opt,
                            argv[optind - 1]);
                    return EXIT_FAILURE;
                } else if (errno == ERANGE) {
                    fprintf(stderr, "%s over- or underflows double \n", argv[optind - 1]);
                    printf("datatype double has to be in range +/- 1.7E +/-308.\n");
                    return EXIT_FAILURE;
                }
                if (!negativeNumber_Set) {
                    negativeNumber_Set = true;
                    negativeNumber_Index = optind;
                    number = newNum;
                }
                break;
            case '?':
                if (optopt == 'V')
                    fprintf(stderr, "Option -V requires an Argument\n");
                else
                    fprintf(stderr, "Unknown Option -%c\n ", optopt);
                return EXIT_FAILURE;
            default:
                fprintf(stderr, "Unknown symbol %c\n", optopt);
                return EXIT_FAILURE;
        }
    }

    if (optind >= argc && !negativeNumber_Set) {
        fprintf(stderr, "Missing positional argument -- ’x’\n");
        printf("Input value needs to be specified.\n");
        return EXIT_FAILURE;
    } else if (!negativeNumber_Set || negativeNumber_Index > optind) {
        number = strtod(argv[optind], &endptr);
        if (endptr == argv[optind] || *endptr != '\0') {
            fprintf(stderr, "%s could not be converted to double. \n", argv[optind]);
            return EXIT_FAILURE;
        } else if (errno == ERANGE) {
            fprintf(stderr, "%s over- or underflows double \n", argv[optind]);
            printf("datatype double has to be in range +/- 1.7E +/-308.\n");
            return EXIT_FAILURE;
        }
    }

    if (relativeError == 0 && iterations == 0) {
        double result = calculate_result(number, implementation);
        printf("Calculating arsinh(%f) with implementation number %li results in %.*g.\n", number, implementation, 20,
               result);
        if (implementation == 1 && (number < -1. || number > 1.)) {
            printf("WARNING: The Taylor Series of Arsinh only provides meaningful results within its convergence zone [-1; 1].\n");
            printf("For all other input values consider using the Implementation -V 2, which uses different series for the approximation.\n");
        }
        return EXIT_SUCCESS;
    } else if (relativeError == 1) {
        double error = relative_error(number, implementation);
        printf("The relative error when calculating arsinh(%f) with implementation number %ld was %.*f\n", number,
               implementation, 15, error);
        return EXIT_SUCCESS;
    } else {
        double time = performance(iterations, number, implementation);
        printf("The measured runtime of %ld iterations of implementation number %ld was %f seconds.\n", iterations,
               implementation, time);
        if (iterations < 100000000) {
            printf("WARNING: Using less than 10000000 iterations is not recommended. \nThe program might not run long enough to provide meaningful measurements.");
        }
        return EXIT_SUCCESS;
    }
}

