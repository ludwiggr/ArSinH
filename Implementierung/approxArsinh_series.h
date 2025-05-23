#include <string.h>
#include <math.h>

double approxArsinh_series(double x);

double approxArsinh_series_V2(double x);


/* Stores precalculated coefficients for the Taylor series of arsinh around 0*/
static double const coeffs_below[] = {1.0, -0.16666666666666666, 0.075, -0.044642857142857144, 0.030381944444444444,
                                      -0.022372159090909092, 0.017352764423076924, -0.01396484375, 0.011551800896139705,
                                      -0.009761609529194078, 0.008390335809616815, -0.0073125258735988454,
                                      0.006447210311889648, -0.005740037670841924, 0.005153309682319904};


/* Stores precalculated coefficients of the Taylor series of the ln around 1*/
static double const coeffs_taylor[] = {1.0, -0.5, 0.3333333333333333, -0.25, 0.2, -0.16666666666666666,
                                       0.14285714285714285, -0.125, 0.1111111111111111, -0.1, 0.09090909090909091,
                                       -0.08333333333333333, 0.07692307692307693, -0.07142857142857142,
                                       0.06666666666666667, -0.0625, 0.058823529411764705, -0.05555555555555555,
                                       0.05263157894736842, -0.05, 0.047619047619047616, -0.045454545454545456,
                                       0.043478260869565216, -0.041666666666666664, 0.04, -0.038461538461538464,
                                       0.037037037037037035, -0.03571428571428571, 0.034482758620689655,
                                       -0.03333333333333333, 0.03225806451612903, -0.03125};


/* Stores precalculated coefficients of the asymptotic expansion rest series */
static double const coeffs_rest[] = {0.25, -0.09375, 0.052083333333333336, -0.0341796875, 0.024609375, -0.018798828125,
                                     0.014962332589285714, -0.012273788452148438, 0.010303921169704862,
                                     -0.008809852600097656, 0.007644913413307883, -0.00671584407488505,
                                     0.00596080835048969, -0.005337356456688472, 0.004815481603145599};




