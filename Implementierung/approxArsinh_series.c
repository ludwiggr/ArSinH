#include "approxArsinh_lookup.h"


#define LOG_TWO log(2.0)

void printOutOfRange(void) {
    printf("Error: Input out of range!\nTry using the lookup table implementation instead.\n");
}

double approxXBelowOne(double x) {      //this is the regular Series for |x|<1 
    unsigned long long mask = 0x7FF0000000000000;
    unsigned long long xToBit = *(unsigned long long*)&x;
    unsigned long long exp = (xToBit & mask) >> 52;
    size_t iterations = 1024 + 2*(exp - 1023);     //the lower the exponent of x is the fewer iterations you need, because x^n will converge to 0 way faster
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

double approxLnInterval(double x) {  //this is the Tayler Series for ln x around 1 (applicable for 0<x<2)
    double fak = -x+1;               //it's more accurate, the closer the number is to 0
    double prev = fak;
    double sum = fak;
    for(size_t k = 2; k<1000; k++){
        prev *= ((double) (k-1) / (double) k) * fak;
        sum += prev;
    }
    return -sum;
}

double approxLn(double x) {       //converts ln(x) to ln(x'*2^n) = ln(x')+n*ln(2) so that the Taylor series can be applied to x
    unsigned long long mask = 0x7FF0000000000000;        //
    unsigned long long xToBit = *(unsigned long long*) &x;
    unsigned long long exp = (xToBit & mask) >> 52;
    unsigned long long n = exp - 1023; //bias of doubles
    unsigned long long xLowerTwo = xToBit | 0x3FF0000000000000;
    double xNew = *(double*) &xLowerTwo;
    if(xNew > 1.3333333333333333){  //this ensures, that x is as close to 1 as possible
        xNew /= 2;                  //so the result is more accurate
        n++;
    }
    return approxLnInterval(xNew) + n * LOG_TWO;
}

double approxAsymptoticExpansionRest(double x) {   
    unsigned long long mask = 0x7FF0000000000000;
    unsigned long long xToBit = *(unsigned long long*)&x;
    unsigned long long exp = (xToBit & mask) >> 52;
    size_t iterations = 1024 - 2*(exp - 1023); //max 1024 iterations -- for greater x, you need less iterations
    if(iterations < 0){                        //if x > 2^512, arsinh(x) can be approximated with ln(2x), therefore this rest is 0
        return 0.;
    }
    double minusInvSqr = -1.0/(x*x);
    double prev = minusInvSqr / (-4);
    double sum = prev;
    for(size_t k = 2; k < iterations + 2; k++){
        size_t num = (2 * k - 1) * (2 * k - 2);
        size_t den = 4 * k * k;
        prev *= ((double) num / (double) den) * minusInvSqr;
        sum += prev;
    }
    return sum;
}

double approxArsinh_series(double x) {
    if(x == -INFINITY || x == INFINITY || x == NAN || x == -NAN){
        return x;
    }
    if(x >= 1){
        return LOG_TWO + approxLn(x) + approxAsymptoticExpansionRest(x);
    }
    if(x>=0){
        return approxXBelowOne(x);
    }
    if(x <= -1){
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






