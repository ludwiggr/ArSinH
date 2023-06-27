#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <getopt.h>
#include <stddef.h>
#include <errno.h>
#include <math.h>
#include <string.h>
#include "approxArsinh_lookup.h"
#include "approxArsinh_series.h"
#include <time.h>

//Kommentar Ludwig: Double ist implementierungs-/ architekturabhängig: Die genauen Eigenschaften und Wertebereiche auf der benutzten Architektur können über die Headerdatei <float.h> ermittelt werden.

static const long int numberOfImplementations = 4;     //number of possible Implementations to choose from
static const long int maxNumberOfRepetitions = 1000000;   //max number of repetitions of the function call to avoid timeout

void help(void) {                            //prints options and use cases of the program
    printf( "-----------------------------------------------------------------------------------------------\n"
            "| Help Desk -- Options and Use Cases Of the Program.                                          |\n"
            "| This program calculates the arsinh(x), which is equal to ln(x + sqrt(x^2 + 1))              |\n"
            "|                                                                                             |\n"
            "| You can parse the value x at any point within the program arguments.                        |\n"
            "| The program will only calculate the result for the first number, that is parsed.            |\n"
            "| When parsing a negative number, make sure to place it after the options and use a           |\n"
            "| double dash beforhand.Else it will be interpreted as an option.                             |\n"
            "| If no valid number is entered, the program will calculate arsinh(0)                         |\n"
            "|                                                                                             |\n"
            "| You can choose between different implementations using the -V option.                       |\n"
            "| You have the following options: (the default implementation is 0)                           |\n"
            "| - 0: LookUp-Table: this approximates the arsinh(x) using a logarithmic lookup-table.        |\n"
            "|      this approximation is significantly faster than the series approximation, but also more|\n"
            "|      inaccurate, since all values in between two values in the lookup-table can only be     |\n"
            "|      estimated linearly.                                                                    |\n"
            "| - 1: Series-Approximation: this uses two different series approximations for the intervals  |\n"
            "|      |x|<1 and |x|>1. Both approximations use a variation of the Taylor series.             |\n"
            "|      The number of iterations and runtime is much higher for values of x, close to 1.       |\n"
            "| - 2: Pre-Build Functions: this approximation uses the predefined functions log and sqrt     |\n"
            "|      from the math-library to calculate the arsinh(x).                                      |\n"
            "|                                                                                             |\n"
            "| You can use the Option -B to run a runtime analysis.                                        |\n"
            "| It requires your wanted number of iterations of the function as an argument and will return |\n"
            "| the total time, the program took for the calculation.                                       |\n"
            "-----------------------------------------------------------------------------------------------\n");
}

double approxArsinh_series(double x);

double approxArsinh_lookup(double x);

double approxArsinh_predefined(double x) {
    if(x>1e16){
        return log(2)+log(x);
    }
    return log(x + sqrt(x * x + 1));
}

double calculate_result(double x, int implementation){  //just calculates the result and returns the double
    switch (implementation) {
        case 0:
            return approxArsinh_lookup(x);
            break;
        case 1:
            return approxArsinh_series(x);
            break;
        case 2:
            return approxArsinh_predefined(x);
            break;
        default:
            fprintf(stderr, "No such implementation.\n");
            return EXIT_FAILURE;
    }
}

double relative_error(double x, int implementation) {      //returns the relative error using the implementation of the math library as comparism
    double exact = approxArsinh_predefined(x);
    double approx = 0.0;
    switch (implementation) {
        case 0:
            approx = approxArsinh_lookup(x);
            break;
        case 1:
            approx = approxArsinh_series(x);
            break;
        case 2: 
            approx = approxArsinh_predefined(x);
            break;
        default:
            printf("Error: Implementation not found!\n");
            return -1;
            break;
    }
    return fabs((approx - exact) / exact);
}

double performance(unsigned int n, double x, int implementation) {
    /*
    n = number of repetitions (should be at least 20)
    x = input value
    implementation =  0: lookup table ; 1: series ; 2: impl with complex functions
    */
    struct timespec start, end;
    int c;
    switch (implementation) {
        case 0:
            c = clock_gettime(CLOCK_MONOTONIC, &start);
            if (c == -1) {
                printf("Error: clock_gettime failed!\n");
                return EXIT_FAILURE;
            }
            for (unsigned int i = 0; i < n; i++) {
                approxArsinh_lookup(x);
            }
            c = clock_gettime(CLOCK_MONOTONIC, &end);
            if (c == -1) {
                printf("Error: clock_gettime failed!\n");
                return EXIT_FAILURE;
            }
            break;
        case 1:
            c = clock_gettime(CLOCK_MONOTONIC, &start);
            if (c == -1) {
                printf("Error: clock_gettime failed!\n");
                return EXIT_FAILURE;
            }
            for (unsigned int i = 0; i < n; i++) {
                approxArsinh_series(x);
            }
            c = clock_gettime(CLOCK_MONOTONIC, &end);
            if (c == -1) {
                printf("Error: clock_gettime failed!\n");
                return EXIT_FAILURE;
            }
            break;

        case 2:
            c = clock_gettime(CLOCK_MONOTONIC, &start);
            if (c == -1) {
                printf("Error: clock_gettime failed!\n");
                return EXIT_FAILURE;
            }
            for (unsigned int i = 0; i < n; i++) {
                approxArsinh_predefined(x);
            }
            c = clock_gettime(CLOCK_MONOTONIC, &end);
            if (c == -1) {
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


//Ludwig: Beachten Sie ebenfalls, dass Ihr Rahmenprogramm etwaige Randfälle korrekt abfangen muss und im Falle eines
// Fehlers mit einer aussagekräftigen Fehlermeldung auf stderr und einer kurzen Erläuterung zur Benutzung terminieren sollte.
// TODO: Sie dürfen weitere Optionen implementieren, beispielsweise um vordefinierte Testfälle zu verwenden.
int main(int argc, char *argv[]) {

    long int implementation = 0;          // := choose Implementation
    long int iterations = 0;              // := if B-flag this sets numbers of iterations for runtime measurement
    double number = 0.;                   // := input value
    size_t relativeError =  0;

    errno = 0;
    char *endptr;
    int opt;
    int option_index = 0;
    struct option long_options[] = {      //converts --help to -h
        {"help", no_argument, NULL, 'h'},
        {NULL, 0, NULL, 0}
    };
    while ((opt = getopt_long(argc, argv, "V:B:hR", long_options, &option_index)) != -1) {
        switch (opt) {
            case 'V':           //choose implementation
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
                else if (implementation < 0 || implementation >= numberOfImplementations) {
                    fprintf(stderr, "%li is not a valid index for an implementation.\nChoose a value between 0 and %li.\n",
                    implementation, numberOfImplementations - 1);
                    return EXIT_FAILURE;
                }
                break;
            case 'B':           //choose number of iterations for runtime measurement
                iterations = strtol(optarg, &endptr, 10);
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
                else if (iterations < 0 || iterations > maxNumberOfRepetitions) {
                    fprintf(stderr,
                    "%li is not a valid number of repetitions of the function call.\nChoose a value between 1 and %li.\n",
                    iterations, maxNumberOfRepetitions);
                    return EXIT_FAILURE;
                }
                break;
            case 'R'  :
                relativeError = 1;
                break;
            case 'h'  :
                help();
                return EXIT_SUCCESS;
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

    if (optind != argc) {  //optional argument can be parsed at any point, unless it's negative
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

    if (relativeError==0 && iterations == 0) {
        double result = calculate_result(number, implementation);
        printf("Calculating arsinh(%f) with implementation number %li results in %f.\n", number, implementation, result);
        return EXIT_SUCCESS;
    }
    else if(relativeError == 1){
        double error = relative_error(number, implementation);
        printf("The relative error when calculating arsinh(%f) with implementation number %ld was %.*f\n", number, implementation, 15, error);
        return EXIT_SUCCESS;
    }
    else {
        double time = performance(iterations, number, implementation);
        printf("The accumilated runtime of %ld iterations of implementation number %ld was %f seconds.\n", iterations,
               implementation, time);
        return EXIT_SUCCESS;
    }
    return 0;
}

