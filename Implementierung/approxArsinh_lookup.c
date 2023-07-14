#include "approxArsinh_lookup.h"

/* An Approximation of the arsinh using a lookup-table
 *
 * Returns: An Approximation of the arsinh(x) for all possible inputs
 */
double approxArsinh_lookup(double x) {
    if (x == -INFINITY || x == INFINITY || x == NAN || x == -NAN) {
        return x;
    }
    signed long long mask = 0x7FFF000000000000;
    signed long long xToBit;
    memcpy(&xToBit, &x, 8);                                 
    signed long long index = (xToBit & mask) >> 48;     //Bithack: get exponent and first 4 bits of mantissa
    double ylow = table[index];
    double yhigh = table[index + 1];
    double relation = (double) (xToBit & 0xFFFFFFFFFFFF) / (double) 0x1000000000000;
    double result = ylow + relation * (yhigh - ylow);   //Bithack: use mask for linear interpolation
    return x >= 0 ? result : -result;                   //use symmetry of arsinh(x)
}

