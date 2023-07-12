#include "approxArsinh_series.h"
#include <string.h>

static const double LOG_TWO = 0.69314718055994530941;  

double approxAsymptoticExpansionRest(double x) {
    size_t iterations = 13;   //fix number of iterations to avoid case distinctions
    double InvSqr = 1.0 / (x * x);  //base for terms of the series
    double prev = InvSqr;
    double sum = 0;
    for (size_t k = 0; k < iterations; k++) {
        sum += prev * coeffs_rest[k];   //use stored coefficients to calculate terms of series
        prev *= InvSqr;                 //update power of base
    }
    return sum;
}

double approxLnTaylor(double x) {  //Tayler Series for ln x around 1 (applicable for 0<x<2)
    double fak = x - 1;               
    double prev = fak;                //use (x-1) as base for the series
    double sum = 0;
    size_t iterations = 27;     //fix number of iterations to avoid case distinctions
            //Since -0.33<=fak<=0.33, after at most 27 iterations of the series, 
            //the terms don't matter for the end result,
            //due to floating point cancellation 
    for (size_t k = 0; k <= iterations; k++) {    
        sum += prev * coeffs_taylor[k]; //use stored coefficients to calculate terms of series
        prev *= fak;                //update power of base
    }
    return sum;         
}

double approxLn(double x) {       //converts ln(x) to ln(x'*2^n) = ln(x')+n*ln(2) 
                //so that the Taylor series can be applied to x
    signed long long mask = 0x7FF0000000000000;        
    signed long long xToBit;
    memcpy(&xToBit, &x, 8);
    signed long long exp = ((xToBit & mask) >> 52) - 1023; //this is the exponent of x
    signed long long xLowerTwo = (xToBit & 0xBFFFFFFFFFFFFFFF) | 0x3FF0000000000000;
    double mantissa;
    memcpy(&mantissa, &xLowerTwo, 8); //uses a bitmask to set the exponent to 0
    if (mantissa > 1.3333333333333333) {  //this ensures, that x is as close to 1 as possible
        mantissa /= 2;                  //so the result is more accurate
        exp++;
    }
    return approxLnTaylor(mantissa) + exp * LOG_TWO; //ln(x) = ln(mantissa * 2^exp) = ln(mantissa) * exp ln(2)
}

double approxXAboveOne(double x){     //uses the approximation arsinh(x) = ln(2x) + error(x)
    if(x>0){ //use symmetry of the arsinh function
        return LOG_TWO + approxLn(x) + approxAsymptoticExpansionRest(x); 
    }
    else{
        return -(LOG_TWO + approxLn(-x) + approxAsymptoticExpansionRest(-x));
    }
    //already converts ln(2x) to ln(2)+ln(x)
}


double approxArsinh_series(double x) { //only uses the series around 0 without cas distinctions
                                    //only returns meaningful results for [-1; +1]
    size_t iterations = 13; //fixed number of series terms to avoid case distinctions
    double sum = 0;
    double prev = x;
    double Sqr = x * x;                 //x^2 as base for terms of the series
    for (size_t k = 0; k <= iterations; k++) {
        sum += prev * coeffs_below[k];  //use stored coefficients to calculate terms of series
        prev *= Sqr;                    //update power of base
    }
    return sum;
}

double approxArsinh_differentSeries(double x) {
    if (x == -INFINITY || x == INFINITY || x == NAN || x == -NAN) { //catch edgecases
        return x;
    } else if (x >= 1 || x<=-1) {   //for |x|>=1
        return approxXAboveOne(x);  //use ln(2x)+error(x)
    } else {                        //for ]-1; +1[
        return approxArsinh_series(x);  //use TaylorSeries of arsinh around 0 
    }
}






