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

int validate_inputs(const double* coeffs, int degree, double initial_guess) {
    if (degree < 1 || degree > 10) return 0;
    if (coeffs == NULL) return 0;
    if (!isfinite(initial_guess)) return 0;
    for (int i = 0; i <= degree; i++) {
        if (!isfinite(coeffs[i])) return 0;
    }
    return 1;
}

double find_root_newton(const double* coeffs, int degree, double initial_guess) {
    if (!validate_inputs(coeffs, degree, initial_guess)) return NAN;
    
    double x = initial_guess;
    for (int iter = 0; iter < MAX_ITERATIONS; iter++) {
        double fx = compute_polynomial(x, coeffs, degree);
        if (fabs(fx) < EPSILON) return x;
        
        double dfx = compute_derivative(x, coeffs, degree);
        if (fabs(dfx) < EPSILON) return NAN;
        
        double delta = fx / dfx;
        if (!isfinite(delta)) return NAN;
        
        x -= delta;
        if (!isfinite(x)) return NAN;
        
        if (fabs(delta) < EPSILON) return x;
    }
    return NAN;
}

int main() {
    double coefficients[] = {2.0, -3.0, 1.0};
    int degree = 2;
    double initial_guesses[] = {0.5, 2.5, -1.0};
    int num_guesses = 3;
    
    printf("Finding roots of polynomial: ");
    for (int i = degree; i >= 0; i--) {
        if (i == degree) {
            printf("%.1fx^%d", coefficients[i], i);
        } else if (i > 1) {
            printf(" + %.1fx^%d", coefficients[i], i);
        } else if (i == 1) {
            printf(" + %.1fx", coefficients[i]);
        } else {
            printf(" + %.1f", coefficients[i]);
        }
    }
    printf("\n\n");
    
    for (int i = 0; i < num_guesses; i++) {
        double root = find_root_newton(coefficients, degree, initial_guesses[i]);
        printf("Initial guess: %.1f -> ", initial_guesses[i]);
        if (isfinite(root)) {
            printf("Root found at x = %.6f\n", root);
            double verification = compute_polynomial(root, coefficients, degree);
            printf("Verification: f(%.6f) = %.6e\n", root, verification);
        } else {
            printf("No root found or method failed\n");
        }
        printf("\n");
    }
    
    return 0;
}