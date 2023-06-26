#include "approxArsinh_lookup.h"


#define LOG_TWO log(2.0)

void printOutOfRange(void) {
    printf("Error: Input out of range!\nTry using the lookup table implementation instead.\n");
}

double approxXBelowOne(double x) {      //this is the regular Series for |x|<1 
    signed long long mask = 0x7FF0000000000000;
    signed long long xToBit = *(unsigned long long*)&x;
    signed long long exp = ((xToBit & mask) >> 52) -1022; //subtract bias, but add one for the calculation of relevant iterations
    size_t iterations = exp<0 ? ((52 / -exp +1) / 2) : 52 + ((xToBit & 0xFFFFFFFFFFFFF) >> 40);  
    //52/-exp is the highest power of x that will not be completely canceled when being added to x
    //the lower the exponent of x is the fewer iterations you need, because x^n will converge to 0 way faster
    //for x close to 1 we need a lot of iterations, because the terms further down the series still influence the endresult
    //with >>40 we set the maximum number of iterations to 2^12, else it would be 2^52
    //with a maximum of 4150 iterations we get a guaranteed accuracy of the result of 20 bit
    double sum = x;
    double prev = x;
    double minusSqrt = -x * x;
    for (size_t k = 1; k < iterations; k++) {
        size_t num = (2 * k - 1) * (2 * k - 1);
        size_t den = (2 * k) * (2 * k + 1);
        prev = prev * (double) num * minusSqrt / (double) den;
        sum += prev;
    }
    return sum;
}

double approxLnTaylor(double x) {  //this is the Tayler Series for ln x around 1 (applicable for 0<x<2)
    double fak = -x+1;               //it's more accurate, the closer the number is to 1
    double prev = fak;
    double sum = fak;
    size_t iterations = 33;     //Since -0.33<=fak<=0.33, after at most 33 iterations of the series, the terms ^don't matter for the end result,
    for(size_t k = 2; k <= iterations; k++){    // due to floating point cancellation (0,33^33 < 2^-52)
        prev *= fak;
        sum += prev / (double) k;
    }
    return -sum;
}

double approxLn(double x) {       //converts ln(x) to ln(x'*2^n) = ln(x')+n*ln(2) so that the Taylor series can be applied to x
    signed long long mask = 0x7FF0000000000000;        //
    signed long long xToBit = *(signed long long*) &x;
    signed long long exp = ((xToBit & mask) >> 52) -1023;
    signed long long xLowerTwo = xToBit & 0xBFFFFFFFFFFFFFFF| 0x3FF0000000000000;
    double xNew = *(double*) &xLowerTwo;
    if(xNew > 1.3333333333333333){  //this ensures, that x is as close to 1 as possible
        xNew /= 2;                  //so the result is more accurate
        exp++;
    }
    return approxLnTaylor(xNew) + exp * LOG_TWO;
}

double approxAsymptoticExpansionRest(double x) {   
    signed long long mask = 0x7FF0000000000000;
    signed long long xToBit = *(signed long long*)&x;
    signed long long exp = ((xToBit & mask) >> 52) - 1023;
    size_t iterations = exp>0 ? 52 / (2*exp) : 52 + (0x1000-((xToBit & 0xFFFFFFFFFFFFF) >> 40));
    //52/exp is the highest power of x that will not be completely canceled when being added to x
    //for x close to 1 we need a lot of iterations, because the terms further down the series still influence the endresult
    //we set the maximum number of iterations to ca. 4150, which gives us a definite accuracy of the result of 20 bit
    if(iterations==0){                        //if x > ca. 10^8, arsinh(x) can be approximated with ln(2x), therefore this rest is 0
        return 0.;
    }
    double minusInvSqr = -1.0/(x*x);
    double prev = minusInvSqr / (-4);
    double sum = prev;
    for(size_t k = 2; k <= iterations; k++){
        size_t num = (2 * k - 1) * (2 * k - 2);
        size_t den = 4 * k * k;
        prev *= ((double) num / (double) den) * minusInvSqr;
        sum += prev;
    }
    return (double) iterations;
}

double approxArsinh_series(double x) {
    if(x == -INFINITY || x == INFINITY || x == NAN || x == -NAN){
        return x;
    }
    else if(x >= 1){
        return approxAsymptoticExpansionRest(x);
        //return LOG_TWO + approxLn(x) + approxAsymptoticExpansionRest(x);
    }
    else if(x>=0){
        return approxXBelowOne(x);
    }
    else if(x <= -1){
        return -(LOG_TWO + approxLn(-x) + approxAsymptoticExpansionRest(-x));
    }
    else return -approxXBelowOne(-x);
    /*if (x < -1.0 || x > 1.0) {
        printOutOfRange();
        return NAN;
    }
    double coeffs[] = {-0.16666666666666666, 0.075, -0.044642857142857144, 0.030381944444444444, -0.022372159090909092,
                       0.017352764423076924, -0.01396484375};
    double sum = x;
    double monom = x;
    for (int i = 0; i < 7; i++) {
        sum += coeffs[i] * monom;
        monom *= x * x;
    }
    return sum;*/
}






