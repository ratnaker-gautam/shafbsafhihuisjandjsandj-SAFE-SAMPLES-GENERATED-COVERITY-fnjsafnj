//DeepSeek-V3 SAFE v1.4 Category: Safe ; Style: modular_functions ; Variation: numeric_computation
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <stdint.h>
#include <limits.h>

#define MAX_ITERATIONS 1000
#define EPSILON 1e-10

double compute_polynomial(double x, const double* coeffs, int degree) {
    if (degree < 0 || coeffs == NULL) return NAN;
    double result = 0.0;
    double term = 1.0;
    for (int i = 0; i <= degree; i++) {
        result += coeffs[i] * term;
        term *= x;
    }
    return result;
}

double compute_derivative(double x, const double* coeffs, int degree) {
    if (degree < 1 || coeffs == NULL) return NAN;
    double result = 0.0;
    double term = 1.0;
    for (int i = 1; i <= degree; i++) {
        result += i * coeffs[i] * term;
        term *= x;
    }
    return result;
}

double newton_raphson(const double* coeffs, int degree, double initial_guess) {
    if (degree < 1 || coeffs == NULL) return NAN;
    if (fabs(initial_guess) > 1e100) return NAN;
    
    double x = initial_guess;
    for (int i = 0; i < MAX_ITERATIONS; i++) {
        double fx = compute_polynomial(x, coeffs, degree);
        if (fabs(fx) < EPSILON) return x;
        
        double fpx = compute_derivative(x, coeffs, degree);
        if (fabs(fpx) < EPSILON) return NAN;
        
        double delta = fx / fpx;
        if (fabs(delta) < EPSILON) return x;
        
        x -= delta;
        if (!isfinite(x)) return NAN;
    }
    return NAN;
}

int read_polynomial(double* coeffs, int* degree) {
    printf("Enter polynomial degree (1-10): ");
    if (scanf("%d", degree) != 1) return 0;
    if (*degree < 1 || *degree > 10) return 0;
    
    printf("Enter %d coefficients from constant to highest degree:\n", *degree + 1);
    for (int i = 0; i <= *degree; i++) {
        if (scanf("%lf", &coeffs[i]) != 1) return 0;
        if (!isfinite(coeffs[i])) return 0;
    }
    return 1;
}

int main(void) {
    double coeffs[11];
    int degree;
    
    printf("Polynomial Root Finder using Newton-Raphson Method\n");
    printf("=================================================\n");
    
    if (!read_polynomial(coeffs, &degree)) {
        printf("Error: Invalid polynomial input\n");
        return 1;
    }
    
    double initial_guess;
    printf("Enter initial guess: ");
    if (scanf("%lf", &initial_guess) != 1) {
        printf("Error: Invalid initial guess\n");
        return 1;
    }
    
    if (!isfinite(initial_guess)) {
        printf("Error: Initial guess must be finite\n");
        return 1;
    }
    
    double root = newton_raphson(coeffs, degree, initial_guess);
    
    if (isnan(root)) {
        printf("Failed to converge to a root\n");
    } else {
        printf("Found root: %.10f\n", root);
        double verification = compute_polynomial(root, coeffs, degree);
        printf("Verification f(root) = %.2e\n", verification);
    }
    
    return 0;
}