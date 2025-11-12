//DeepSeek-V3 V2.5 Category: Safe ; Style: modular_functions ; Variation: numeric_computation
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#define MAX_ITERATIONS 1000
#define EPSILON 1e-10

double compute_polynomial(double x, double* coeffs, int degree) {
    double result = 0.0;
    double term = 1.0;
    
    for (int i = 0; i <= degree; i++) {
        result += coeffs[i] * term;
        term *= x;
    }
    
    return result;
}

double compute_derivative(double x, double* coeffs, int degree) {
    double result = 0.0;
    double term = 1.0;
    
    for (int i = 1; i <= degree; i++) {
        result += i * coeffs[i] * term;
        term *= x;
    }
    
    return result;
}

int validate_coefficients(double* coeffs, int degree) {
    if (degree < 0 || degree > 10) {
        return 0;
    }
    
    for (int i = 0; i <= degree; i++) {
        if (!isfinite(coeffs[i])) {
            return 0;
        }
    }
    
    return 1;
}

double find_root_newton(double* coeffs, int degree, double initial_guess) {
    if (!validate_coefficients(coeffs, degree)) {
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
        
        if (fabs(delta) < EPSILON) {
            break;
        }
        
        x = x - delta;
        
        if (!isfinite(x)) {
            return NAN;
        }
        
        iterations++;
    }
    
    if (iterations >= MAX_ITERATIONS) {
        return NAN;
    }
    
    return x;
}

int main() {
    double coefficients[11] = {0};
    int degree;
    double initial_guess;
    
    printf("Enter polynomial degree (0-10): ");
    if (scanf("%d", &degree) != 1) {
        printf("Invalid input for degree\n");
        return 1;
    }
    
    if (degree < 0 || degree > 10) {
        printf("Degree must be between 0 and 10\n");
        return 1;
    }
    
    printf("Enter %d coefficients (from constant term to highest degree):\n", degree + 1);
    for (int i = 0; i <= degree; i++) {
        if (scanf("%lf", &coefficients[i]) != 1) {
            printf("Invalid input for coefficient %d\n", i);
            return 1;
        }
    }
    
    printf("Enter initial guess: ");
    if (scanf("%lf", &initial_guess) != 1) {
        printf("Invalid input for initial guess\n");
        return 1;
    }
    
    if (!isfinite(initial_guess)) {
        printf("Initial guess must be finite\n");
        return 1;
    }
    
    double root = find_root_newton(coefficients, degree, initial_guess);
    
    if (isnan(root)) {
        printf("Failed to find root within %d iterations\n", MAX_ITERATIONS);
        return 1;
    }
    
    double verification = compute_polynomial(root, coefficients, degree);
    
    printf("Found root: %.10f\n", root);
    printf("Polynomial value at root: %.10f\n", verification);
    
    return 0;
}