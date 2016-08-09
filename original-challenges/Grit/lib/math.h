#ifndef MATH_H_
#define MATH_H_

#define NAN __builtin_nan("")
#define PI 3.14159265358979323846264338327950288

static inline double cgc_fmin(double a, double b)
{
    return a < b ? a : b;
}

static inline double cgc_fmax(double a, double b)
{
    return a > b ? a : b;
}

#endif
