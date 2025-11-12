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

double newton_raphson(double* coeffs, int degree, double initial_guess) {
    if (!validate_coefficients(coeffs, degree)) {
        return NAN;
    }
    
    if (!isfinite(initial_guess)) {
        return NAN;
    }
    
    double x = initial_guess;
    double fx, dfx;
    int iterations = 0;
    
    while (iterations < MAX_ITERATIONS) {
        fx = compute_polynomial(x, coeffs, degree);
        dfx = compute_derivative(x, coeffs, degree);
        
        if (fabs(dfx) < EPSILON) {
            return NAN;
        }
        
        double delta = fx / dfx;
        
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

int read_polynomial(double* coeffs, int* degree) {
    printf("Enter polynomial degree (0-10): ");
    if (scanf("%d", degree) != 1) {
        return 0;
    }
    
    if (*degree < 0 || *degree > 10) {
        return 0;
    }
    
    printf("Enter %d coefficients (from constant term to highest degree):\n", *degree + 1);
    for (int i = 0; i <= *degree; i++) {
        if (scanf("%lf", &coeffs[i]) != 1) {
            return 0;
        }
    }
    
    return 1;
}

int main(void) {
    double coeffs[11];
    int degree;
    double initial_guess;
    
    printf("Polynomial Root Finder using Newton-Raphson Method\n");
    printf("=================================================\n");
    
    if (!read_polynomial(coeffs, &degree)) {
        printf("Error: Invalid polynomial input.\n");
        return 1;
    }
    
    printf("Enter initial guess: ");
    if (scanf("%lf", &initial_guess) != 1) {
        printf("Error: Invalid initial guess.\n");
        return 1;
    }
    
    double root = newton_raphson(coeffs, degree, initial_guess);
    
    if (isnan(root)) {
        printf("Failed to find root within %d iterations.\n", MAX_ITERATIONS);
        return 1;
    }
    
    printf("Found root: %.10f\n", root);
    printf("Polynomial value at root: %.10f\n", compute_polynomial(root, coeffs, degree));
    
    return 0;
}