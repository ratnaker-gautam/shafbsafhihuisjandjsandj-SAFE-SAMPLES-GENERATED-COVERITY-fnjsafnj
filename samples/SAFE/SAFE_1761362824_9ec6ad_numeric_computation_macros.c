//DeepSeek-V3 V2.5 Category: Safe ; Style: macros ; Variation: numeric_computation
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <stdint.h>
#include <limits.h>
#include <float.h>

#define MAX_ITERATIONS 1000
#define EPSILON 1e-10
#define MAX_INPUT 1000000.0
#define MIN_INPUT -1000000.0
#define IS_VALID(x) ((x) >= MIN_INPUT && (x) <= MAX_INPUT)

double safe_pow(double base, int exp) {
    if (exp == 0) return 1.0;
    if (base == 0.0) return 0.0;
    
    double result = 1.0;
    int abs_exp = abs(exp);
    
    for (int i = 0; i < abs_exp; i++) {
        if (result > DBL_MAX / fabs(base)) {
            return (exp > 0) ? DBL_MAX : 0.0;
        }
        result *= base;
    }
    
    return (exp > 0) ? result : 1.0 / result;
}

double compute_polynomial(double x, double coeffs[], int degree) {
    double result = 0.0;
    for (int i = 0; i <= degree; i++) {
        double term = safe_pow(x, i) * coeffs[i];
        if (fabs(result) > DBL_MAX - fabs(term)) {
            return (result > 0) ? DBL_MAX : -DBL_MAX;
        }
        result += term;
    }
    return result;
}

int find_root_bisection(double coeffs[], int degree, double a, double b, double *root) {
    if (!IS_VALID(a) || !IS_VALID(b) || a >= b) {
        return 0;
    }
    
    double fa = compute_polynomial(a, coeffs, degree);
    double fb = compute_polynomial(b, coeffs, degree);
    
    if (!IS_VALID(fa) || !IS_VALID(fb)) {
        return 0;
    }
    
    if (fa * fb > 0) {
        return 0;
    }
    
    for (int i = 0; i < MAX_ITERATIONS; i++) {
        double mid = a + (b - a) / 2.0;
        if (!IS_VALID(mid)) {
            return 0;
        }
        
        double fmid = compute_polynomial(mid, coeffs, degree);
        if (!IS_VALID(fmid)) {
            return 0;
        }
        
        if (fabs(fmid) < EPSILON || (b - a) / 2.0 < EPSILON) {
            *root = mid;
            return 1;
        }
        
        if (fa * fmid < 0) {
            b = mid;
            fb = fmid;
        } else {
            a = mid;
            fa = fmid;
        }
    }
    
    return 0;
}

int main() {
    double coeffs[4] = {1.0, -6.0, 11.0, -6.0};
    int degree = 3;
    
    printf("Finding roots of polynomial: x^3 - 6x^2 + 11x - 6\n");
    printf("Enter search interval [a,b]: ");
    
    double a, b;
    if (scanf("%lf %lf", &a, &b) != 2) {
        printf("Error: Invalid input format\n");
        return 1;
    }
    
    if (!IS_VALID(a) || !IS_VALID(b)) {
        printf("Error: Input values out of valid range [%g, %g]\n", MIN_INPUT, MAX_INPUT);
        return 1;
    }
    
    if (a >= b) {
        printf("Error: a must be less than b\n");
        return 1;
    }
    
    double root;
    if (find_root_bisection(coeffs, degree, a, b, &root)) {
        printf("Found root at x = %.10g\n", root);
        printf("f(%.10g) = %.10g\n", root, compute_polynomial(root, coeffs, degree));
    } else {
        printf("No root found in the given interval or error occurred\n");
    }
    
    return 0;
}