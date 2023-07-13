#include "approxArsinh_lookup.h"

/* An Approximation of the arsinh using a lookup-table
 *
 * Returns: An Approximation of the arsinh(x)
 */
double approxArsinh_lookup(double x) {
    if (x == -INFINITY || x == INFINITY || x == NAN || x == -NAN) {
        return x;
    }
    signed long long mask = 0x7FFF000000000000;
    signed long long xToBit;
    memcpy(&xToBit, &x, 8);                                 //Bithack: get exponent and first 4 bits of mantissa
    signed long long index = (xToBit & mask) >> 48;
    double ylow = table[index];
    double yhigh = table[index + 1];
    double relation = (double) (xToBit & 0xFFFFFFFFFFFF) / (double) 0x1000000000000;
    double result = ylow + relation * (yhigh - ylow);
    return x >= 0 ? result : -result;
}

