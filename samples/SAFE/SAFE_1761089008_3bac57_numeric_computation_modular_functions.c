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

double newton_raphson(const double* coeffs, int degree, double initial_guess) {
    if (degree < 1 || coeffs == NULL) return NAN;
    if (fabs(initial_guess) > 1e100) return NAN;
    
    double x = initial_guess;
    for (int i = 0; i < MAX_ITERATIONS; i++) {
        double fx = compute_polynomial(x, coeffs, degree);
        if (fabs(fx) < EPSILON) return x;
        
        double fpx = compute_derivative(x, coeffs, degree);
        if (fabs(fpx) < EPSILON) return NAN;
        
        double delta = fx / fpx;
        if (fabs(delta) < EPSILON) return x;
        
        x -= delta;
        if (fabs(x) > 1e100) return NAN;
    }
    return NAN;
}

int get_valid_degree() {
    int degree;
    printf("Enter polynomial degree (1-10): ");
    if (scanf("%d", &degree) != 1) return -1;
    if (degree < 1 || degree > 10) return -1;
    return degree;
}

int get_coefficients(double* coeffs, int degree) {
    printf("Enter %d coefficients (from constant to highest degree):\n", degree + 1);
    for (int i = 0; i <= degree; i++) {
        printf("Coefficient %d: ", i);
        if (scanf("%lf", &coeffs[i]) != 1) return 0;
    }
    return 1;
}

double get_initial_guess() {
    double guess;
    printf("Enter initial guess: ");
    if (scanf("%lf", &guess) != 1) return NAN;
    return guess;
}

int main() {
    int degree = get_valid_degree();
    if (degree == -1) {
        printf("Invalid degree input.\n");
        return 1;
    }
    
    double coefficients[11];
    if (!get_coefficients(coefficients, degree)) {
        printf("Invalid coefficient input.\n");
        return 1;
    }
    
    double initial_guess = get_initial_guess();
    if (isnan(initial_guess)) {
        printf("Invalid initial guess.\n");
        return 1;
    }
    
    double root = newton_raphson(coefficients, degree, initial_guess);
    
    if (isnan(root)) {
        printf("Failed to find root within %d iterations.\n", MAX_ITERATIONS);
    } else {
        printf("Approximate root: %.10f\n", root);
        double verification = compute_polynomial(root, coefficients, degree);
        printf("Verification f(root) = %.10f\n", verification);
    }
    
    return 0;
}