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
    if (degree > 20) return -1;
    
    double x = initial_guess;
    int iterations = 0;
    
    while (iterations < MAX_ITERATIONS) {
        double fx = compute_polynomial(x, coeffs, degree);
        double fpx = compute_derivative(x, coeffs, degree);
        
        if (fabs(fpx) < EPSILON) return -1;
        
        double delta = fx / fpx;
        if (fabs(delta) < EPSILON) {
            *root = x;
            return iterations;
        }
        
        x -= delta;
        iterations++;
    }
    
    return -1;
}

int read_polynomial_coeffs(double* coeffs, int max_degree) {
    if (coeffs == NULL || max_degree < 0 || max_degree > 20) return -1;
    
    int degree;
    printf("Enter polynomial degree (0-20): ");
    if (scanf("%d", &degree) != 1) return -1;
    if (degree < 0 || degree > max_degree) return -1;
    
    printf("Enter %d coefficients (from constant term to highest degree): ", degree + 1);
    for (int i = 0; i <= degree; i++) {
        if (scanf("%lf", &coeffs[i]) != 1) return -1;
    }
    
    return degree;
}

int main() {
    double coeffs[21];
    double initial_guess;
    double root;
    
    int degree = read_polynomial_coeffs(coeffs, 20);
    if (degree < 0) {
        printf("Error: Invalid polynomial input\n");
        return 1;
    }
    
    printf("Enter initial guess for root: ");
    if (scanf("%lf", &initial_guess) != 1) {
        printf("Error: Invalid initial guess\n");
        return 1;
    }
    
    if (!isfinite(initial_guess)) {
        printf("Error: Initial guess must be finite\n");
        return 1;
    }
    
    int iterations = find_polynomial_root(coeffs, degree, initial_guess, &root);
    
    if (iterations >= 0) {
        printf("Found root: %.10f\n", root);
        printf("Function value at root: %.10f\n", compute_polynomial(root, coeffs, degree));
        printf("Iterations required: %d\n", iterations);
    } else {
        printf("Failed to find root within %d iterations\n", MAX_ITERATIONS);
    }
    
    return 0;
}