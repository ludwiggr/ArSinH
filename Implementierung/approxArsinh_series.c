#include "approxArsinh_series.h"
#include <string.h>

static const double LOG_TWO = 0.69314718055994530941;
// static const double ROOT_TWO = 0x3FF6A09E667F3BCD;

void printOutOfRange(void) {
    printf("Error: Input out of range!\nTry using the lookup table implementation instead.\n");
}

double approxXBelowOne(double x) {      //this is the regular Series for |x|<1 
    //signed long long mask = 0x7FF0000000000000;
    //signed long long xToBit;
    //memcpy(&xToBit, &x, 8);
    //signed long long exp = ((xToBit & mask) >> 52) -1022; //subtract bias, but add one for the calculation of relevant iterations
    size_t iterations = 13;//exp<0 ? ((52 / -exp +1) / 2) : 26; 
    //52/-exp is the highest power of x that will not be completely canceled when being added to x
    //the lower the exponent of x is the fewer iterations you need, because x^n will converge to 0 way faster
    //for x close to 1 we need a lot of iterations, because the terms further down the series still influence the endresult
    //with >>40 we set the maximum number of iterations to 2^12, else it would be 2^52
    //with a maximum of 4150 iterations we get a guaranteed accuracy of the result of 20 bit
    double sum = 0;
    double prev = x;
    double Sqr = x * x;
    for (size_t k = 0; k <= iterations; k++) {
        sum += prev * coeffs_below[k];
        prev *= Sqr;
    }
    return sum;
}

double approxLnTaylor(double x) {  //this is the Tayler Series for ln x around 1 (applicable for 0<x<2)
    double fak = x - 1;               //it's more accurate, the closer the number is to 1
    double prev = fak;
    double sum = 0;
    size_t iterations = 27;     //Since -0.33<=fak<=0.33, after at most 33 iterations of the series, the terms ^don't matter for the end result,
    for (size_t k = 0; k <= iterations; k++) {    // due to floating point cancellation (0,33^33 < 2^-52)
        sum += prev * coeffs_taylor[k];
        prev *= fak;
    }
    return sum;
}

double approxLn(double x) {       //converts ln(x) to ln(x'*2^n) = ln(x')+n*ln(2) so that the Taylor series can be applied to x
    signed long long mask = 0x7FF0000000000000;        //
    signed long long xToBit;
    memcpy(&xToBit, &x, 8);
    signed long long exp = ((xToBit & mask) >> 52) - 1023;
    signed long long xLowerTwo = (xToBit & 0xBFFFFFFFFFFFFFFF) | 0x3FF0000000000000;
    double mantissa;
    memcpy(&mantissa, &xLowerTwo, 8);
    if (mantissa > 1.3333333333333333) {  //this ensures, that x is as close to 1 as possible
        mantissa /= 2;                  //so the result is more accurate
        exp++;
    }
    return approxLnTaylor(mantissa) + exp * LOG_TWO;
}

double approxAsymptoticExpansionRest(double x) {
    //signed long long mask = 0x7FF0000000000000;
    //signed long long xToBit;
    //memcpy(&xToBit, &x, 8);
    //signed long long exp = ((xToBit & mask) >> 52) - 1023;
    size_t iterations = 13; //exp>0 ? 52 / (2*exp) : 26;
    //52/exp is the highest power of x that will not be completely canceled when being added to x
    //for x close to 1 we need a lot of iterations, because the terms further down the series still influence the endresult
    //we set the maximum number of iterations to ca. 4150, which gives us a definite accuracy of the result of 20 bit
    if (iterations ==
        0) {                        //if x > ca. 10^8, arsinh(x) can be approximated with ln(2x), therefore this rest is 0
        return 0.;
    }
    double InvSqr = 1.0 / (x * x);
    double prev = InvSqr;
    double sum = 0;
    for (size_t k = 0; k < iterations; k++) {
        sum += prev * coeffs_rest[k];
        prev *= InvSqr;
    }
    return sum;
}


double approx_root(double x, size_t iterations){
    // signed long long mask = 0x7FF0000000000000;        //
    // signed long long xToBit;
    // double root = 0;
    // memcpy(&xToBit, &x, 8);
    // signed long long exp = xToBit & mask;
    // signed long long xLowerTwo = (xToBit & 0xBFFFFFFFFFFFFFFF) | 0x3FF0000000000000;
    // if(exp&0x001 == 1){
    //     root = ROOT_TWO;
    // }
    // double mantissa;
    // memcpy(&mantissa, &xLowerTwo, 8);
    double x0 = (x + 1)/4 + x/(x + 1);

    for(size_t i = 0;i<iterations;i++){
        x0 = (x0 + x/x0)/2;
    }


    return x0;
}

double approxArsinh_series2(double x) {
    if (x == -INFINITY || x == INFINITY || x == NAN || x == -NAN) {
        return x;
    }
    else if(x<0){
        return -(approxLn(x + approx_root(x*x + 1, 3)));
    }
    else if(x<10){
        return approxLn(x + approx_root(x*x + 1, 3));
    }
    return LOG_TWO + approxLn(x);
}

double approxArsinh_series(double x) {
    if (x == -INFINITY || x == INFINITY || x == NAN || x == -NAN) {
        return x;
    } else if (x >= 1) {
        return LOG_TWO + approxLn(x) + approxAsymptoticExpansionRest(x);
    } else if (x >= 0) {
        return approxXBelowOne(x);
    } else if (x <= -1) {
        return -(LOG_TWO + approxLn(-x) + approxAsymptoticExpansionRest(-x));
    } else return -approxXBelowOne(-x);
}








