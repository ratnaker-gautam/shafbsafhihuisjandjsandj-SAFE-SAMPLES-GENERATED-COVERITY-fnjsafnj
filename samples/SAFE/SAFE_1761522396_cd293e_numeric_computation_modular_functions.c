//DeepSeek-V3 V2.5 Category: Safe ; Style: modular_functions ; Variation: numeric_computation
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

int find_polynomial_root(const double* coeffs, int degree, double initial_guess, double* root) {
    if (degree < 1 || coeffs == NULL || root == NULL) return -1;
    
    double x = initial_guess;
    int iterations = 0;
    
    while (iterations < MAX_ITERATIONS) {
        double fx = compute_polynomial(x, coeffs, degree);
        double fpx = compute_derivative(x, coeffs, degree);
        
        if (fabs(fpx) < EPSILON) return -2;
        
        double delta = fx / fpx;
        x = x - delta;
        
        if (fabs(delta) < EPSILON) {
            *root = x;
            return iterations;
        }
        
        iterations++;
    }
    
    return -3;
}

int read_polynomial_coefficients(double* coeffs, int max_degree) {
    if (coeffs == NULL || max_degree < 0) return -1;
    
    int degree;
    printf("Enter polynomial degree (1-%d): ", max_degree);
    if (scanf("%d", &degree) != 1) return -1;
    
    if (degree < 1 || degree > max_degree) return -1;
    
    printf("Enter %d coefficients from highest to lowest degree:\n", degree + 1);
    for (int i = degree; i >= 0; i--) {
        printf("Coefficient for x^%d: ", i);
        if (scanf("%lf", &coeffs[i]) != 1) return -1;
    }
    
    return degree;
}

int main() {
    const int MAX_DEGREE = 10;
    double coefficients[MAX_DEGREE + 1];
    
    int degree = read_polynomial_coefficients(coefficients, MAX_DEGREE);
    if (degree < 0) {
        printf("Error: Invalid polynomial input.\n");
        return 1;
    }
    
    double initial_guess;
    printf("Enter initial guess for root finding: ");
    if (scanf("%lf", &initial_guess) != 1) {
        printf("Error: Invalid initial guess.\n");
        return 1;
    }
    
    double root;
    int result = find_polynomial_root(coefficients, degree, initial_guess, &root);
    
    if (result >= 0) {
        printf("Found root at x = %.10f after %d iterations.\n", root, result);
        printf("Polynomial value at root: %.10f\n", compute_polynomial(root, coefficients, degree));
    } else if (result == -1) {
        printf("Error: Invalid parameters.\n");
    } else if (result == -2) {
        printf("Error: Derivative too small, cannot continue.\n");
    } else {
        printf("Error: Maximum iterations reached, no convergence.\n");
    }
    
    return 0;
}