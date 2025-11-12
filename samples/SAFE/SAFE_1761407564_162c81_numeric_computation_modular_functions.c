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
        if (isinf(term) || isnan(term)) return NAN;
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
        if (isinf(term) || isnan(term)) return NAN;
        result += i * coeffs[i] * term;
        term *= x;
    }
    return result;
}

int find_root_newton(double* root, const double* coeffs, int degree, double initial_guess) {
    if (root == NULL || coeffs == NULL || degree < 1) return -1;
    if (isnan(initial_guess) || isinf(initial_guess)) return -1;
    
    double x = initial_guess;
    for (int iter = 0; iter < MAX_ITERATIONS; iter++) {
        double fx = compute_polynomial(x, coeffs, degree);
        double dfx = compute_derivative(x, coeffs, degree);
        
        if (isnan(fx) || isnan(dfx) || isinf(fx) || isinf(dfx)) return -1;
        if (fabs(dfx) < EPSILON) return -1;
        
        double delta = fx / dfx;
        if (isnan(delta) || isinf(delta)) return -1;
        
        x -= delta;
        if (fabs(delta) < EPSILON) {
            *root = x;
            return iter + 1;
        }
    }
    return -1;
}

int read_polynomial(double* coeffs, int* degree) {
    if (coeffs == NULL || degree == NULL) return -1;
    
    printf("Enter polynomial degree (1-10): ");
    if (scanf("%d", degree) != 1) return -1;
    if (*degree < 1 || *degree > 10) return -1;
    
    printf("Enter %d coefficients from constant to highest degree:\n", *degree + 1);
    for (int i = 0; i <= *degree; i++) {
        if (scanf("%lf", &coeffs[i]) != 1) return -1;
        if (isnan(coeffs[i]) || isinf(coeffs[i])) return -1;
    }
    
    return 0;
}

int main(void) {
    double coeffs[11];
    int degree;
    double initial_guess;
    double root;
    
    if (read_polynomial(coeffs, &degree) != 0) {
        printf("Error: Invalid polynomial input\n");
        return EXIT_FAILURE;
    }
    
    printf("Enter initial guess: ");
    if (scanf("%lf", &initial_guess) != 1) {
        printf("Error: Invalid initial guess\n");
        return EXIT_FAILURE;
    }
    
    if (isnan(initial_guess) || isinf(initial_guess)) {
        printf("Error: Invalid initial guess\n");
        return EXIT_FAILURE;
    }
    
    int iterations = find_root_newton(&root, coeffs, degree, initial_guess);
    if (iterations == -1) {
        printf("Error: Failed to find root\n");
        return EXIT_FAILURE;
    }
    
    printf("Root found: %.10f\n", root);
    printf("Iterations: %d\n", iterations);
    printf("Residual: %.2e\n", compute_polynomial(root, coeffs, degree));
    
    return EXIT_SUCCESS;
}