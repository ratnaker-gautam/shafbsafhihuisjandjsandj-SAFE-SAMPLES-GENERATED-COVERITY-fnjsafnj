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
    return exp(x) / (1.0 + SQUARE(x));
}

double find_root_bisection(double a, double b) {
    if (!VALIDATE_INPUT(a) || !VALIDATE_INPUT(b)) {
        return NAN;
    }
    
    if (compute_polynomial(a) * compute_polynomial(b) >= 0) {
        return NAN;
    }
    
    double mid;
    for (int i = 0; i < MAX_ITERATIONS; i++) {
        mid = (a + b) / 2.0;
        double f_mid = compute_polynomial(mid);
        
        if (fabs(f_mid) < EPSILON) {
            return mid;
        }
        
        if (compute_polynomial(a) * f_mid < 0) {
            b = mid;
        } else {
            a = mid;
        }
    }
    
    return mid;
}

int main() {
    double start, end;
    
    printf("Enter interval start: ");
    if (scanf("%lf", &start) != 1) {
        printf("Invalid input\n");
        return EXIT_FAILURE;
    }
    
    printf("Enter interval end: ");
    if (scanf("%lf", &end) != 1) {
        printf("Invalid input\n");
        return EXIT_FAILURE;
    }
    
    if (!VALIDATE_INPUT(start) || !VALIDATE_INPUT(end)) {
        printf("Input out of valid range [%g, %g]\n", MIN_INPUT, MAX_INPUT);
        return EXIT_FAILURE;
    }
    
    if (start >= end) {
        printf("Start must be less than end\n");
        return EXIT_FAILURE;
    }
    
    double root = find_root_bisection(start, end);
    
    if (isnan(root)) {
        printf("No root found in the given interval or invalid interval\n");
    } else {
        printf("Approximate root: %.10f\n", root);
        printf("Function value at root: %.10f\n", compute_polynomial(root));
        printf("Exponential function value: %.10f\n", compute_exponential(root));
    }
    
    return EXIT_SUCCESS;
}