//DeepSeek-V3 V2.5 Category: Safe ; Style: modular_functions ; Variation: numeric_computation
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <stdint.h>
#include <limits.h>

#define MAX_ITERATIONS 1000
#define EPSILON 1e-10

double compute_polynomial(double x, double* coeffs, int degree) {
    if (degree < 0 || coeffs == NULL) return NAN;
    
    double result = 0.0;
    double term = 1.0;
    
    for (int i = 0; i <= degree; i++) {
        result += coeffs[i] * term;
        term *= x;
    }
    
    return result;
}

double compute_derivative(double x, double* coeffs, int degree) {
    if (degree < 1 || coeffs == NULL) return NAN;
    
    double result = 0.0;
    double term = 1.0;
    
    for (int i = 1; i <= degree; i++) {
        result += i * coeffs[i] * term;
        term *= x;
    }
    
    return result;
}

int find_root_newton(double* coeffs, int degree, double initial_guess, double* root) {
    if (coeffs == NULL || root == NULL || degree < 1) return 0;
    
    double x = initial_guess;
    int iterations = 0;
    
    while (iterations < MAX_ITERATIONS) {
        double fx = compute_polynomial(x, coeffs, degree);
        double fpx = compute_derivative(x, coeffs, degree);
        
        if (fabs(fpx) < EPSILON) return 0;
        
        double delta = fx / fpx;
        x = x - delta;
        
        if (fabs(delta) < EPSILON) {
            *root = x;
            return 1;
        }
        
        iterations++;
    }
    
    return 0;
}

int read_polynomial_coefficients(double** coeffs, int* degree) {
    if (coeffs == NULL || degree == NULL) return 0;
    
    printf("Enter polynomial degree (1-10): ");
    if (scanf("%d", degree) != 1) return 0;
    
    if (*degree < 1 || *degree > 10) return 0;
    
    *coeffs = malloc((*degree + 1) * sizeof(double));
    if (*coeffs == NULL) return 0;
    
    printf("Enter %d coefficients from constant term to highest degree:\n", *degree + 1);
    for (int i = 0; i <= *degree; i++) {
        if (scanf("%lf", &(*coeffs)[i]) != 1) {
            free(*coeffs);
            return 0;
        }
    }
    
    return 1;
}

int main(void) {
    double* coefficients = NULL;
    int degree = 0;
    double initial_guess = 0.0;
    double root = 0.0;
    
    printf("Polynomial Root Finder using Newton's Method\n");
    printf("===========================================\n\n");
    
    if (!read_polynomial_coefficients(&coefficients, &degree)) {
        printf("Error: Invalid polynomial input.\n");
        return 1;
    }
    
    printf("Enter initial guess: ");
    if (scanf("%lf", &initial_guess) != 1) {
        printf("Error: Invalid initial guess.\n");
        free(coefficients);
        return 1;
    }
    
    printf("\nFinding root...\n");
    
    if (find_root_newton(coefficients, degree, initial_guess, &root)) {
        printf("Root found: %.10f\n", root);
        printf("Function value at root: %.10f\n", compute_polynomial(root, coefficients, degree));
    } else {
        printf("Failed to find root within %d iterations.\n", MAX_ITERATIONS);
    }
    
    free(coefficients);
    return 0;
}