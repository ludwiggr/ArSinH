#include <stdio.h>
#include <math.h>
#include "approxArsinh_lookup.h"
#include "approxArsinh_series.h"
#include "approxArsinh_complexInstructions.h"

/* Calls the Accuracy Tests and returns the measured Values
 *
 * Returns: The accuracy test results
 */
int main() {

    FILE *fp1, *fp2, *fp3, *fp4, *fp5;
    fp1 = fopen("lookup.txt", "w+");
    fp2 = fopen("series.txt", "w+");
    fp3 = fopen("x.txt", "w+");
    fp4 = fopen("predefined.txt", "w+");
    fp5 = fopen("differentSeries.txt", "w+");

    for (int i = -1000; i < 1024; i++) {
        double b = pow(2, i);
        for (int j = 0; j < 20; j++) {
            double x = b + j * (b / 20);
            double lookup = approxArsinh_lookup(x);
            double series = approxArsinh_series(x);
            double predefined = approxArsinh_complexInstructions(x);
            double diff = approxArsinh_differentSeries(x);
            fprintf(fp1, "%.15g, ", lookup);
            fprintf(fp2, "%.15g, ", series);
            fprintf(fp3, "%.15g, ", x);
            fprintf(fp4, "%.15g, ", predefined);
            fprintf(fp5, "%.15g, ", diff);
        }
    }
}