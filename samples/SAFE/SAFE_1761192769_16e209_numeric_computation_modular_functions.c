//DeepSeek-V3 V2.5 Category: Safe ; Style: modular_functions ; Variation: numeric_computation
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#define MAX_ITERATIONS 1000
#define EPSILON 1e-10

double compute_polynomial(double x, double* coeffs, int degree) {
    if (coeffs == NULL || degree < 0) {
        return NAN;
    }
    
    double result = 0.0;
    double term = 1.0;
    
    for (int i = 0; i <= degree; i++) {
        result += coeffs[i] * term;
        term *= x;
    }
    
    return result;
}

double compute_derivative(double x, double* coeffs, int degree) {
    if (coeffs == NULL || degree < 1) {
        return NAN;
    }
    
    double result = 0.0;
    double term = 1.0;
    
    for (int i = 1; i <= degree; i++) {
        result += i * coeffs[i] * term;
        term *= x;
    }
    
    return result;
}

double find_root_newton(double* coeffs, int degree, double initial_guess) {
    if (coeffs == NULL || degree < 1) {
        return NAN;
    }
    
    double x = initial_guess;
    int iterations = 0;
    
    while (iterations < MAX_ITERATIONS) {
        double fx = compute_polynomial(x, coeffs, degree);
        double fpx = compute_derivative(x, coeffs, degree);
        
        if (fabs(fpx) < EPSILON) {
            return NAN;
        }
        
        double delta = fx / fpx;
        x = x - delta;
        
        if (fabs(delta) < EPSILON) {
            return x;
        }
        
        iterations++;
    }
    
    return NAN;
}

int read_polynomial_coefficients(double* coeffs, int max_degree) {
    if (coeffs == NULL || max_degree < 0) {
        return -1;
    }
    
    printf("Enter polynomial degree (0-%d): ", max_degree);
    int degree;
    if (scanf("%d", &degree) != 1) {
        return -1;
    }
    
    if (degree < 0 || degree > max_degree) {
        return -1;
    }
    
    printf("Enter %d coefficients (from constant term to highest degree):\n", degree + 1);
    for (int i = 0; i <= degree; i++) {
        if (scanf("%lf", &coeffs[i]) != 1) {
            return -1;
        }
    }
    
    return degree;
}

int main(void) {
    const int max_degree = 10;
    double coefficients[max_degree + 1];
    
    printf("Polynomial Root Finder using Newton's Method\n");
    printf("===========================================\n\n");
    
    int degree = read_polynomial_coefficients(coefficients, max_degree);
    if (degree < 0) {
        printf("Error: Invalid polynomial input.\n");
        return 1;
    }
    
    if (degree == 0) {
        printf("Constant polynomial has no roots (unless it's zero).\n");
        return 0;
    }
    
    printf("Enter initial guess for root: ");
    double initial_guess;
    if (scanf("%lf", &initial_guess) != 1) {
        printf("Error: Invalid initial guess.\n");
        return 1;
    }
    
    double root = find_root_newton(coefficients, degree, initial_guess);
    
    if (isnan(root)) {
        printf("Failed to find root within %d iterations.\n", MAX_ITERATIONS);
    } else {
        printf("Found root: %.10f\n", root);
        double verification = compute_polynomial(root, coefficients, degree);
        printf("Verification f(root) = %.2e\n", verification);
    }
    
    return 0;
}