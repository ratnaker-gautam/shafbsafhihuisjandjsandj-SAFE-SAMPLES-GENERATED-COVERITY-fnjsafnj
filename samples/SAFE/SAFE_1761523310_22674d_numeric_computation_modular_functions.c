//DeepSeek-V3 V2.5 Category: Safe ; Style: modular_functions ; Variation: numeric_computation
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <stdint.h>
#include <limits.h>
#include <float.h>

#define MAX_ITERATIONS 1000

int validate_input_range(double value, double min, double max) {
    if (value < min || value > max) {
        return 0;
    }
    return 1;
}

double compute_polynomial(double x, double coeffs[], int degree) {
    double result = 0.0;
    double term = 1.0;
    
    for (int i = 0; i <= degree; i++) {
        if (i > 0) {
            if (fabs(x) > 1.0 && term > DBL_MAX / fabs(x)) {
                return NAN;
            }
            term *= x;
        }
        
        if (fabs(coeffs[i]) > DBL_MAX - fabs(result)) {
            return NAN;
        }
        result += coeffs[i] * term;
    }
    
    return result;
}

double compute_derivative(double x, double coeffs[], int degree) {
    if (degree < 1) {
        return 0.0;
    }
    
    double result = 0.0;
    double term = 1.0;
    
    for (int i = 1; i <= degree; i++) {
        if (i > 1) {
            if (fabs(x) > 1.0 && term > DBL_MAX / fabs(x)) {
                return NAN;
            }
            term *= x;
        }
        
        double derivative_term = coeffs[i] * i * term;
        if (fabs(derivative_term) > DBL_MAX - fabs(result)) {
            return NAN;
        }
        result += derivative_term;
    }
    
    return result;
}

double find_root_newton(double coeffs[], int degree, double initial_guess, double tolerance) {
    if (!validate_input_range(tolerance, 1e-15, 1e-3)) {
        return NAN;
    }
    
    double x = initial_guess;
    double prev_x = x;
    
    for (int iter = 0; iter < MAX_ITERATIONS; iter++) {
        double fx = compute_polynomial(x, coeffs, degree);
        double fpx = compute_derivative(x, coeffs, degree);
        
        if (isnan(fx) || isnan(fpx) || fabs(fpx) < tolerance) {
            return NAN;
        }
        
        double delta = fx / fpx;
        if (fabs(delta) > DBL_MAX / 2.0) {
            return NAN;
        }
        
        prev_x = x;
        x = x - delta;
        
        if (fabs(x - prev_x) < tolerance) {
            return x;
        }
    }
    
    return NAN;
}

int main() {
    double coefficients[] = {2.0, -3.0, 1.0};
    int degree = 2;
    double initial_guess = 1.5;
    double tolerance = 1e-10;
    
    printf("Finding root of polynomial: ");
    for (int i = degree; i >= 0; i--) {
        if (i == degree) {
            printf("%.1fx^%d", coefficients[i], i);
        } else if (i > 1) {
            printf(" + %.1fx^%d", coefficients[i], i);
        } else if (i == 1) {
            printf(" + %.1fx", coefficients[i]);
        } else {
            printf(" + %.1f", coefficients[i]);
        }
    }
    printf("\n");
    
    printf("Initial guess: %.1f\n", initial_guess);
    printf("Tolerance: %.1e\n", tolerance);
    
    double root = find_root_newton(coefficients, degree, initial_guess, tolerance);
    
    if (!isnan(root)) {
        printf("Found root: %.10f\n", root);
        double verification = compute_polynomial(root, coefficients, degree);
        printf("Verification f(root) = %.2e\n", verification);
    } else {
        printf("Failed to find root within %d iterations\n", MAX_ITERATIONS);
    }
    
    return 0;
}