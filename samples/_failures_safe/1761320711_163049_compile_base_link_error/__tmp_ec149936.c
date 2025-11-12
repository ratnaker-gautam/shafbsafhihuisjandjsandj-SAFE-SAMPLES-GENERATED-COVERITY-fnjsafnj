//DeepSeek-V3 V2.5 Category: Safe ; Style: macros ; Variation: numeric_computation
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <stdint.h>
#include <limits.h>

#define MAX_ITERATIONS 1000
#define EPSILON 1e-10
#define MAX_INPUT 1000000.0
#define MIN_INPUT -1000000.0

#define VALIDATE_INPUT(x) ((x) >= MIN_INPUT && (x) <= MAX_INPUT)
#define SQUARE(x) ((x) * (x))
#define CUBE(x) ((x) * (x) * (x))

double compute_polynomial(double x) {
    return 3.0 * SQUARE(x) - 2.0 * x + 5.0;
}

double compute_exponential(double x) {
    return exp(x / 2.0);
}

double compute_trigonometric(double x) {
    return sin(x) + cos(x);
}

double find_root_bisection(double a, double b) {
    if (!VALIDATE_INPUT(a) || !VALIDATE_INPUT(b)) {
        return NAN;
    }
    
    if (a >= b) {
        return NAN;
    }
    
    double fa = compute_polynomial(a);
    double fb = compute_polynomial(b);
    
    if (fa * fb > 0) {
        return NAN;
    }
    
    for (int i = 0; i < MAX_ITERATIONS; i++) {
        double mid = a + (b - a) / 2.0;
        double fmid = compute_polynomial(mid);
        
        if (fabs(fmid) < EPSILON || fabs(b - a) < EPSILON) {
            return mid;
        }
        
        if (fa * fmid < 0) {
            b = mid;
            fb = fmid;
        } else {
            a = mid;
            fa = fmid;
        }
    }
    
    return NAN;
}

int main() {
    double start, end;
    
    printf("Enter interval start: ");
    if (scanf("%lf", &start) != 1) {
        printf("Invalid input\n");
        return 1;
    }
    
    printf("Enter interval end: ");
    if (scanf("%lf", &end) != 1) {
        printf("Invalid input\n");
        return 1;
    }
    
    if (!VALIDATE_INPUT(start) || !VALIDATE_INPUT(end)) {
        printf("Input out of valid range [%g, %g]\n", MIN_INPUT, MAX_INPUT);
        return 1;
    }
    
    double root = find_root_bisection(start, end);
    
    if (isnan(root)) {
        printf("No root found in the given interval or invalid interval\n");
    } else {
        printf("Approximate root: %.10f\n", root);
        printf("Polynomial value at root: %.10f\n", compute_polynomial(root));
        printf("Exponential value at root: %.10f\n", compute_exponential(root));
        printf("Trigonometric value at root: %.10f\n", compute_trigonometric(root));
    }
    
    return 0;
}