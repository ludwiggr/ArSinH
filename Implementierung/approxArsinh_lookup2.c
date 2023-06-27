#include "approxArsinh_lookup2.h"
#include <string.h>

double approxArsinh_lookup2 (double x){
    signed long long mask = 0x7FFF000000000000;
    signed long long xToBit;
    memcpy(&xToBit, &x, 8);
    signed long long Index = (xToBit & mask) >> 48;
    double ylow = table[Index];
    double yhigh = table[Index+1];
    double relation = (double)(xToBit & 0xFFFFFFFFFFFF) / (double) 0x1000000000000;
    return ylow + relation * (yhigh - ylow);

}