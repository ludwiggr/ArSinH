#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include <stddef.h>
#include <errno.h>
#include <math.h>

static const long int numberOfImplementations= 3;     //number of possible Implementations to choose from
static const long int maxNumberOfRepetitions= 1000;   //max number of repetitions of the function call to avoid timeout

void help(void){                            //prints options and use cases of the program
    printf("get some help\n");
}
double approxArsinh_series(double x){return x;}
double approxArsinh_lookup(double x){return x;}
double approxArsinh_predefined(double x){
    return log(x+sqrt(x*x+1));
}


int main (int argc, char *argv[]) {
    const char* progname = argv[0];

    long int implementation= 0;          //choose Implementation 
    long int runtime = 0;                 //if B-flag this sets numbers of executions for runtime measurement
    double number = 0.;                 //x

    errno = 0;
    char* endptr;

    int opt;
    while((opt = getopt(argc, argv, "V:B:h"))!=-1){
        switch(opt){
            case 'V':
                implementation = strtol(optarg, &endptr, 10);
                if(endptr == optarg || *endptr != '\0') {
                    fprintf(stderr, "%s could not be converted to long. It doesn't contain any valid digits.\n", optarg) ;
                    return EXIT_FAILURE ;
                }
                else if (errno == ERANGE) {
                    fprintf ( stderr , "%s over - or underflows long \n", optarg) ;
                    return EXIT_FAILURE ;
                }
                break;
            case 'B':
                runtime = strtol(optarg, &endptr, 10);
                if(endptr == optarg || *endptr != '\0') {
                    fprintf(stderr, "%s could not be converted to long. It doesn't contain any valid digits. \n", optarg) ;
                    return EXIT_FAILURE ;
                }
                else if (errno == ERANGE) {
                    fprintf ( stderr , "%s over - or underflows long \n", optarg) ;
                    return EXIT_FAILURE ;
                }
                break;
            case 'h'  :
                help();
                return EXIT_SUCCESS;
            case '?':
                if (optopt=='V'||optopt=='B')
                    fprintf(stderr, "Options -V and -B require an Argument\n");
                else 
                    fprintf(stderr, "Unknown Option -%c\n", optopt);
                return EXIT_FAILURE;
            default: 
                return EXIT_FAILURE;
        }
    }
    if(optind != argc) {
        number = strtod(argv[optind], &endptr);
        if(endptr == argv[optind] || *endptr != '\0') {
            fprintf(stderr, "%s could not be converted to double. \n ", argv [optind]) ;
            return EXIT_FAILURE ;
        }
        else if (errno == ERANGE) {
            fprintf ( stderr , "%s over - or underflows double \n", argv [optind]) ;
            return EXIT_FAILURE ;
        }
    }
    if(implementation<0 || implementation>=numberOfImplementations){
        fprintf(stderr, "%li is not a valid index for an implementation. Choose a value between 0 and %li.\n", 
        implementation,  numberOfImplementations-1 );
        return EXIT_FAILURE;
    }
    if(runtime<0 || runtime>maxNumberOfRepetitions){
        fprintf(stderr, "%li is not a valid number of repetitions of the function call. Choose a value between 1 and %li.\n", 
        runtime,  maxNumberOfRepetitions);
        return EXIT_FAILURE;
    }

    //printf("Implementation has value: %li\n", implementation);
    //printf("Runtime has value: %li\n", runtime);
    //printf("number has value: %f\n", number);
    double result;
    if(runtime==0){
        switch(implementation){
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
            default:
                fprintf(stderr, "No such implementation.\n");
                return EXIT_FAILURE;
        }
    }
    else {
        fprintf(stderr, "Runtime-measurement not yet implemented\n");
        return EXIT_FAILURE;
    }
    return 0;
}

