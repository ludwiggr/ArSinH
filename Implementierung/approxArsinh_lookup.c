
#include "approxArsinh_lookup.h"

int exponent(double d){
    size_t e = (int)(d&&0x7FF0000000000000);
    e = e >> 52;
    e = e - 1023;
    return e;
}

double approxArsinh_lookup(double x) {
    if (x == 0.0) {
        return 0;
    }
    if (x < 0) {
        return -approxArsinh_lookup(-x);
    }
    int exp = exponent(x);
    //check if is nan or inf
    if (exp == 1024) {
        return NAN;
    }
    if (exp <= 0) {
        return x;
    }
    double m = (ytable[exp] - ytable[exp - 1]) / (xtable[exp] - xtable[exp - 1]);
    return m * (x - xtable[exp - 1]) + ytable[exp - 1];
}
