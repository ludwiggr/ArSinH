#include "approxArsinh_lookup.h"
#include "approxArsinh_series.h"
#include "approxArsinh_complexInstructions.h"
#include <time.h>

/* Measures and Computes the performance of a given arsinh implementation compared to the math library
 *
 * Returns: the relative error for arsinh(x) for the Argument n (number of executions) the Arguments x (Argument of the function) and the Argument implementation [0;3]
 */
double performance(unsigned int n, double x, int implementation);
