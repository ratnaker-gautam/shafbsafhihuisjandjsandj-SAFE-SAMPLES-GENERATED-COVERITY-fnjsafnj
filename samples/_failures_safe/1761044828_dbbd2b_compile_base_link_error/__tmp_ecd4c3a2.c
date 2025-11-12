//DeepSeek-V3 SAFE v1.4 Category: Safe ; Style: macros ; Variation: numeric_computation
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <stdint.h>
#include <limits.h>

#define MAX_ITERATIONS 1000
#define EPSILON 1e-10
#define MAX_INPUT 1000000.0
#define MIN_INPUT 0.0001

double safe_pow(double base, double exponent) {
    if (base < 0.0 && fmod(exponent, 1.0) != 0.0) {
        return NAN;
    }
    if (base == 0.0 && exponent < 0.0) {
        return INFINITY;
    }
    return pow(base, exponent);
}

double compute_function(double x) {
    double term1 = safe_pow(x, 2.5);
    if (isnan(term1) || isinf(term1)) {
        return NAN;
    }
    
    double term2 = log(fabs(x) + 1.0);
    if (isnan(term2) || isinf(term2)) {
        return NAN;
    }
    
    double term3 = sin(x * 0.5);
    if (isnan(term3)) {
        return NAN;
    }
    
    return term1 + term2 + term3;
}

double find_root_bisection(double a, double b) {
    if (a >= b) {
        return NAN;
    }
    
    double fa = compute_function(a);
    double fb = compute_function(b);
    
    if (isnan(fa) || isnan(fb)) {
        return NAN;
    }
    
    if (fa * fb > 0.0) {
        return NAN;
    }
    
    for (int i = 0; i < MAX_ITERATIONS; i++) {
        double mid = a + (b - a) / 2.0;
        double fmid = compute_function(mid);
        
        if (isnan(fmid) || isinf(fmid)) {
            return NAN;
        }
        
        if (fabs(fmid) < EPSILON || (b - a) / 2.0 < EPSILON) {
            return mid;
        }
        
        if (fa * fmid < 0.0) {
            b = mid;
            fb = fmid;
        } else {
            a = mid;
            fa = fmid;
        }
    }
    
    return NAN;
}

int main(void) {
    double lower_bound, upper_bound;
    
    printf("Enter lower bound for root finding: ");
    if (scanf("%lf", &lower_bound) != 1) {
        printf("Invalid input for lower bound\n");
        return EXIT_FAILURE;
    }
    
    printf("Enter upper bound for root finding: ");
    if (scanf("%lf", &upper_bound) != 1) {
        printf("Invalid input for upper bound\n");
        return EXIT_FAILURE;
    }
    
    if (lower_bound >= upper_bound) {
        printf("Lower bound must be less than upper bound\n");
        return EXIT_FAILURE;
    }
    
    if (fabs(lower_bound) > MAX_INPUT || fabs(upper_bound) > MAX_INPUT) {
        printf("Input values too large\n");
        return EXIT_FAILURE;
    }
    
    if (fabs(lower_bound) < MIN_INPUT && fabs(lower_bound) > 0.0) {
        printf("Input values too small\n");
        return EXIT_FAILURE;
    }
    
    if (fabs(upper_bound) < MIN_INPUT && fabs(upper_bound) > 0.0) {
        printf("Input values too small\n");
        return EXIT_FAILURE;
    }
    
    double root = find_root_bisection(lower_bound, upper_bound);
    
    if (isnan(root)) {
        printf("No root found in the given interval or invalid interval\n");
        return EXIT_FAILURE;
    }
    
    printf("Found root at: %.10f\n", root);
    printf("Function value at root: %.10f\n", compute_function(root));
    
    return EXIT_SUCCESS;
}