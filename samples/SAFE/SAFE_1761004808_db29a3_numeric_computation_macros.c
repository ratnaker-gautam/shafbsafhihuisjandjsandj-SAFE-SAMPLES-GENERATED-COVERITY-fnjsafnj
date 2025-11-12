//DeepSeek-V3 SAFE v1.4 Category: Safe ; Style: macros ; Variation: numeric_computation
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <limits.h>
#include <float.h>

#define MAX_ITERATIONS 1000
#define EPSILON 1e-10
#define MAX_INPUT 1000000.0
#define MIN_INPUT -1000000.0

double safe_power(double base, int exponent) {
    if (exponent == 0) return 1.0;
    if (base == 0.0) return 0.0;
    
    double result = 1.0;
    int abs_exp = exponent < 0 ? -exponent : exponent;
    
    for (int i = 0; i < abs_exp; i++) {
        if (result > DBL_MAX / fabs(base)) {
            return exponent > 0 ? DBL_MAX : 0.0;
        }
        result *= base;
    }
    
    return exponent < 0 ? 1.0 / result : result;
}

double compute_polynomial(double x, double coeffs[], int degree) {
    if (degree < 0 || degree > 100) return 0.0;
    
    double result = 0.0;
    for (int i = 0; i <= degree; i++) {
        double term = coeffs[i] * safe_power(x, i);
        if (fabs(result) > DBL_MAX - fabs(term)) {
            return result > 0 ? DBL_MAX : -DBL_MAX;
        }
        result += term;
    }
    return result;
}

double find_root_bisection(double coeffs[], int degree, double a, double b) {
    if (degree < 1) return NAN;
    if (a >= b) return NAN;
    
    double fa = compute_polynomial(a, coeffs, degree);
    double fb = compute_polynomial(b, coeffs, degree);
    
    if (fa * fb > 0) return NAN;
    
    for (int iter = 0; iter < MAX_ITERATIONS; iter++) {
        double mid = a + (b - a) / 2.0;
        if (b - a < EPSILON) return mid;
        
        double fmid = compute_polynomial(mid, coeffs, degree);
        
        if (fabs(fmid) < EPSILON) return mid;
        
        if (fa * fmid < 0) {
            b = mid;
            fb = fmid;
        } else {
            a = mid;
            fa = fmid;
        }
    }
    
    return a + (b - a) / 2.0;
}

int main() {
    double coefficients[4] = {2.0, -3.0, 1.0, -5.0};
    int degree = 3;
    
    double lower_bound, upper_bound;
    
    printf("Enter lower bound for root search: ");
    if (scanf("%lf", &lower_bound) != 1) {
        printf("Invalid input for lower bound\n");
        return 1;
    }
    
    printf("Enter upper bound for root search: ");
    if (scanf("%lf", &upper_bound) != 1) {
        printf("Invalid input for upper bound\n");
        return 1;
    }
    
    if (lower_bound >= upper_bound) {
        printf("Lower bound must be less than upper bound\n");
        return 1;
    }
    
    if (lower_bound < MIN_INPUT || lower_bound > MAX_INPUT || 
        upper_bound < MIN_INPUT || upper_bound > MAX_INPUT) {
        printf("Bounds must be between %.1f and %.1f\n", MIN_INPUT, MAX_INPUT);
        return 1;
    }
    
    double root = find_root_bisection(coefficients, degree, lower_bound, upper_bound);
    
    if (isnan(root)) {
        printf("No root found in the given interval or invalid parameters\n");
    } else {
        printf("Approximate root found at: %.10f\n", root);
        printf("Polynomial value at root: %.10f\n", compute_polynomial(root, coefficients, degree));
    }
    
    return 0;
}