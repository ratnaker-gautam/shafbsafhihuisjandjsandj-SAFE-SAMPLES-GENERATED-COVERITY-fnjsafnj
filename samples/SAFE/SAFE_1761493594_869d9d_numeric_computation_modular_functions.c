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
        if (isinf(term) || isnan(term)) {
            return NAN;
        }
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
        if (isinf(term) || isnan(term)) {
            return NAN;
        }
        result += i * coeffs[i] * term;
        term *= x;
    }
    
    return result;
}

int find_root_newton(double* coeffs, int degree, double initial_guess, double* root) {
    if (coeffs == NULL || degree < 1 || root == NULL) {
        return -1;
    }
    
    double x = initial_guess;
    int iterations = 0;
    
    while (iterations < MAX_ITERATIONS) {
        double fx = compute_polynomial(x, coeffs, degree);
        double fpx = compute_derivative(x, coeffs, degree);
        
        if (isnan(fx) || isnan(fpx) || fabs(fpx) < EPSILON) {
            return -1;
        }
        
        double delta = fx / fpx;
        if (fabs(delta) < EPSILON) {
            *root = x;
            return 0;
        }
        
        x = x - delta;
        iterations++;
        
        if (isinf(x) || isnan(x)) {
            return -1;
        }
    }
    
    return -1;
}

int read_polynomial(double** coeffs, int* degree) {
    if (coeffs == NULL || degree == NULL) {
        return -1;
    }
    
    printf("Enter polynomial degree (1-10): ");
    if (scanf("%d", degree) != 1) {
        return -1;
    }
    
    if (*degree < 1 || *degree > 10) {
        return -1;
    }
    
    *coeffs = malloc((*degree + 1) * sizeof(double));
    if (*coeffs == NULL) {
        return -1;
    }
    
    printf("Enter %d coefficients from constant to highest degree: ", *degree + 1);
    for (int i = 0; i <= *degree; i++) {
        if (scanf("%lf", &(*coeffs)[i]) != 1) {
            free(*coeffs);
            return -1;
        }
    }
    
    return 0;
}

int main(void) {
    double* coefficients = NULL;
    int degree = 0;
    double initial_guess = 0.0;
    double root = 0.0;
    
    printf("Polynomial Root Finder using Newton's Method\n");
    printf("===========================================\n");
    
    if (read_polynomial(&coefficients, &degree) != 0) {
        printf("Error: Invalid polynomial input.\n");
        return 1;
    }
    
    printf("Enter initial guess: ");
    if (scanf("%lf", &initial_guess) != 1) {
        printf("Error: Invalid initial guess.\n");
        free(coefficients);
        return 1;
    }
    
    if (isinf(initial_guess) || isnan(initial_guess)) {
        printf("Error: Invalid initial guess.\n");
        free(coefficients);
        return 1;
    }
    
    if (find_root_newton(coefficients, degree, initial_guess, &root) == 0) {
        printf("Found root: %.10f\n", root);
        printf("Function value at root: %.10f\n", compute_polynomial(root, coefficients, degree));
    } else {
        printf("Error: Could not find root within %d iterations.\n", MAX_ITERATIONS);
    }
    
    free(coefficients);
    return 0;
}