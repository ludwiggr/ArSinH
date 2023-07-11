#include "approxArsinh_lookup.h"
#include <string.h>

double approxArsinh_lookup(double x) {
    if (x == -INFINITY || x == INFINITY || x == NAN || x == -NAN) { //catch edgecases
        return x;
    }     
    signed long long mask = 0x7FFF000000000000;                     
    signed long long xToBit;
    memcpy(&xToBit, &x, 8);     //get exponent and first 4 bits of mantissa
    signed long long Index = (xToBit & mask) >> 48;      //as table index
    double ylow = table[Index];                         //table entry before x
    double yhigh = table[Index + 1];                    //table entry after x
    double relation = (double) (xToBit & 0xFFFFFFFFFFFF) / (double) 0x1000000000000; 
    double result = ylow + relation * (yhigh - ylow);  //use linear interpolation to approximate
    return x >= 0 ? result : -result;               //use point symmetry for negative values
}