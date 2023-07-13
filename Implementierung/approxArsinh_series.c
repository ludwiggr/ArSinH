#include "approxArsinh_series.h"

/* Non-changing global variable for log_2 */
static const double LOG_TWO = 0.69314718055994530941;

/* Approximation of the Remainder of the Asymptotic Expansion
 *
 * Features: Fixed number of iterations -> No case distinction
 *
 * Returns:
 */
double approxAsymptoticExpansionRest(double x) {
    size_t iterations = 13;
    double InvSqr = 1.0 / (x * x);
    double prev = InvSqr;
    double sum = 0;
    for (size_t k = 0; k < iterations; k++) {
        sum += prev * coeffs_rest[k];
        prev *= InvSqr;
    }
    return sum;
}

/* Computes the Taylor Series for ln(x) close to the value 1
 *
 * Features: Fixed number of iterations -> No case distinction
 * Applicable for 0 < x < 2 After at most 27 iterations of the series, the terms don't matter for
 * the end result, due to floating point cancellation
 * Since -0.33<=fak<=0.33, after at most 27 iterations of the series, the terms don't matter for the end result, due to floating point cancellation
 *
 * Returns: Taylor Series for ln(x) around 1 (applicable for 0<x<2)
 */
double approxLnTaylor(double x) {  //
    double fak = x - 1;
    double prev = fak;
    double sum = 0;
    size_t iterations = 27;
    for (size_t k = 0; k <= iterations; k++) {
        sum += prev * coeffs_taylor[k];
        prev *= fak;
    }
    return sum;
}


/* Converts ln(x) to ln(x'*2^n) = ln(x')+n*ln(2) so that the Taylor series can be applied to x
 *
 * Features: Bitmask,
 *
 * Returns: ln(x) = ln(mantissa * 2^exp) = ln(mantissa) * exp ln(2)
 */
double approxLn(double x) {
    signed long long mask = 0x7FF0000000000000;
    signed long long xToBit;
    memcpy(&xToBit, &x, 8);
    signed long long exp = ((xToBit & mask) >> 52) - 1023;
    signed long long xLowerTwo = (xToBit & 0xBFFFFFFFFFFFFFFF) | 0x3FF0000000000000;
    double mantissa;
    memcpy(&mantissa, &xLowerTwo, 8);
    if (mantissa > 1.3333333333333333) {
        mantissa /= 2;
        exp++;
    }
    return approxLnTaylor(mantissa) + exp * LOG_TWO;
}

/* Uses the approximation arsinh(x) = ln(2x) + error(x)
 *
 * Features: Bitmask, use symmetry of the arsinh function
 * already converts ln(2x) to ln(2)+ln(x)
 *
 * Returns: ln(x) = ln(mantissa * 2^exp) = ln(mantissa) * exp ln(2)
 */
double approxXAboveOne(double x) {
    if (x > 0) {
        return LOG_TWO + approxLn(x) + approxAsymptoticExpansionRest(x);
    } else {
        return -(LOG_TWO + approxLn(-x) + approxAsymptoticExpansionRest(-x));
    }
}


/* Series Approximation of arsinh for [-1; +1]
 *
 * Features: fixed number of series terms to avoid case distinctions
 * x^2 as base for terms of the series
 * only uses the series around 0 without cas distinctions
 *
 * Returns: An approximation of arsinh for [-1 ; +1]
 */
double approxArsinh_series(double x) {
    size_t iterations = 13;
    double sum = 0;
    double prev = x;
    double Sqr = x * x;
    for (size_t k = 0; k <= iterations; k++) {
        sum += prev * coeffs_below[k];
        prev *= Sqr;
    }
    return sum;
}

/* Series Approximation of arsinh for [-inf ; +inf]
 *
 * Features: Uses case distinction for better series Approximation
 *
 * Returns: An approximation of arsinh for [-inf ; +inf]
 */
double approxArsinh_differentSeries(double x) {
    if (x == -INFINITY || x == INFINITY || x == NAN || x == -NAN) {
        return x;
    } else if (x >= 1 || x <= -1) {
        return approxXAboveOne(x);
    } else {
        return approxArsinh_series(x);
    }
}






