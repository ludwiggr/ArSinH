#include "approxArsinh_lookup.h"

void printOutOfRange(void) {
    printf("Error: Input out of range!\nTry using the lookup table implementation instead.\n");
}


double approxArsinh_series(double x) {
    if (x < -1.0 || x > 1.0) {
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
    return sum;
}



