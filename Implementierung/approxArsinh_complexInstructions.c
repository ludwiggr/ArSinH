#include "approxArsinh_complexInstructions.h"

/* Calls the pre-defined Functions log and sqrt in <math.h> to compute the arsinh
 * Uses approximation ln(2x) for greater x-values to avoid overflows
 *
 * Input: any double
 * 
 * Returns: an exact result with maximum double accuracy for |x| > 2^-26
 *  inaccurate values for |x| < 2^-26 due to floating point cancellation
 */
double approxArsinh_complexInstructions(double x) {
    if (x > 1e16) {
        return log(2) + log(x);
    }
    return log(x + sqrt(x * x + 1));
}


