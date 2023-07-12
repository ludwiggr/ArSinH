#include "approxArsinh_lookup.h"
#include <stdio.h>
#include "approxArsinh_series.h"
#include <math.h>

double approxArsinh_predefined(double x) {
    if (x > 1e16) {
        return log(2) + log(x);
    }
    return log(x + sqrt(x * x + 1));
}

int main() {

    FILE *fp1, *fp2, *fp3, *fp4, *fp5;
    fp1 = fopen("lookup.txt", "w+");
    fp2 = fopen("series.txt", "w+");
    fp3 = fopen("x.txt", "w+");
    fp4 = fopen("predefined.txt", "w+");
    fp5 = fopen("differentSeries.txt", "w+");

    for(int i = -1000; i<1024; i++){
        double b = pow(2, i);
        for(int j = 0; j<20; j++){
            double x = b + j * (b/20);
            double lookup = approxArsinh_lookup(x);
            double series = approxArsinh_series(x);
            double predefined = approxArsinh_predefined(x);
            double diff = approxArsinh_differentSeries(x);
            fprintf(fp1, "%.15g, ", lookup);
            fprintf(fp2, "%.15g, ", series);
            fprintf(fp3, "%.15g, ", x);
            fprintf(fp4, "%.15g, ", predefined);
            fprintf(fp5, "%.15g, ", diff);
        }
    }

    /*double m = 1.3;
    while (x < 1e300) {
        double lookup = approxArsinh_lookup(x);
        double series = approxArsinh_series(x);
        double predefined = approxArsinh_predefined(x);
        fprintf(fp1, "%.*f, ", 15, lookup);
        fprintf(fp2, "%.*f, ", 15, series);
        fprintf(fp3, "%.*f, ", 15, x);
        fprintf(fp4, "%.*f, ", 15, predefined);
        x = x * m;
    }*/
}