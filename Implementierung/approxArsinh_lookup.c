#include "approxArsinh_lookup.h"
#include <string.h>

double approxArsinh_lookup (double x){
    if(x == -INFINITY || x == INFINITY || x == NAN || x == -NAN){
        return x;
    }
    signed long long mask = 0x7FFF000000000000;
    signed long long xToBit;
    memcpy(&xToBit, &x, 8);
    signed long long Index = (xToBit & mask) >> 48;
    double ylow = table[Index];
    double yhigh = table[Index+1];
    double relation = (double)(xToBit & 0xFFFFFFFFFFFF) / (double) 0x1000000000000;
    double result = ylow + relation * (yhigh - ylow);
    return x>=0 ? result : -result;
}