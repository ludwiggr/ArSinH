#include "approxArsinh_complexInstructions.h"

/* Calls the pre-defined Functions log and sqrt in <math.h> to compute the arsinh
 *
 * Returns: An approximation of the arsinh
 */
double approxArsinh_complexInstructions(double x) {
    if (x > 1e16) {
        return log(2) + log(x);
    }
    return log(x + sqrt(x * x + 1));
}


