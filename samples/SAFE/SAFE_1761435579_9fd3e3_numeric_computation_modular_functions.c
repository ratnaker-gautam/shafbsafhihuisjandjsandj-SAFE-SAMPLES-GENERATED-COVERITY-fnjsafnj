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

int validate_polynomial_degree(int degree) {
    if (degree < 0 || degree > 10) {
        return 0;
    }
    return 1;
}

int validate_coefficient(double coeff) {
    if (coeff < -1e6 || coeff > 1e6) {
        return 0;
    }
    return 1;
}

int read_polynomial(double* coeffs, int* degree) {
    printf("Enter polynomial degree (0-10): ");
    if (scanf("%d", degree) != 1) {
        return 0;
    }
    
    if (!validate_polynomial_degree(*degree)) {
        return 0;
    }
    
    printf("Enter %d coefficients from constant to highest degree:\n", *degree + 1);
    for (int i = 0; i <= *degree; i++) {
        if (scanf("%lf", &coeffs[i]) != 1) {
            return 0;
        }
        if (!validate_coefficient(coeffs[i])) {
            return 0;
        }
    }
    
    return 1;
}

double find_root_newton(double* coeffs, int degree, double initial_guess) {
    double x = initial_guess;
    double fx, fpx;
    int iterations = 0;
    
    while (iterations < MAX_ITERATIONS) {
        fx = compute_polynomial(x, coeffs, degree);
        fpx = compute_derivative(x, coeffs, degree);
        
        if (fabs(fpx) < EPSILON) {
            break;
        }
        
        double delta = fx / fpx;
        if (fabs(delta) < EPSILON) {
            break;
        }
        
        x = x - delta;
        iterations++;
    }
    
    return x;
}

int main() {
    double coeffs[11];
    int degree;
    double initial_guess;
    
    printf("Polynomial Root Finder using Newton's Method\n");
    printf("===========================================\n\n");
    
    if (!read_polynomial(coeffs, &degree)) {
        printf("Error: Invalid polynomial input\n");
        return 1;
    }
    
    printf("Enter initial guess: ");
    if (scanf("%lf", &initial_guess) != 1) {
        printf("Error: Invalid initial guess\n");
        return 1;
    }
    
    if (initial_guess < -1e6 || initial_guess > 1e6) {
        printf("Error: Initial guess out of range\n");
        return 1;
    }
    
    double root = find_root_newton(coeffs, degree, initial_guess);
    double value_at_root = compute_polynomial(root, coeffs, degree);
    
    printf("\nResults:\n");
    printf("Approximate root: %.10f\n", root);
    printf("Polynomial value at root: %.10f\n", value_at_root);
    printf("Residual: %.2e\n", fabs(value_at_root));
    
    return 0;
}