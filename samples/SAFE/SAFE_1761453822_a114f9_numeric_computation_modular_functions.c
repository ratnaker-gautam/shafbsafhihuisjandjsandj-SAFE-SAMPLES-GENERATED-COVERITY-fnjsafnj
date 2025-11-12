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

double newton_raphson(const double* coeffs, int degree, double initial_guess) {
    if (degree < 1 || coeffs == NULL || !isfinite(initial_guess)) return NAN;
    
    double x = initial_guess;
    for (int i = 0; i < MAX_ITERATIONS; i++) {
        double fx = compute_polynomial(x, coeffs, degree);
        double fpx = compute_derivative(x, coeffs, degree);
        
        if (fabs(fpx) < EPSILON) break;
        if (!isfinite(fx) || !isfinite(fpx)) break;
        
        double delta = fx / fpx;
        if (!isfinite(delta)) break;
        
        x -= delta;
        if (fabs(delta) < EPSILON) break;
    }
    return x;
}

int read_coefficients(double* coeffs, int max_degree) {
    if (coeffs == NULL || max_degree < 0) return -1;
    
    printf("Enter polynomial degree (0-%d): ", max_degree);
    int degree;
    if (scanf("%d", &degree) != 1) return -1;
    if (degree < 0 || degree > max_degree) return -1;
    
    printf("Enter %d coefficients from constant to highest term:\n", degree + 1);
    for (int i = 0; i <= degree; i++) {
        if (scanf("%lf", &coeffs[i]) != 1) return -1;
        if (!isfinite(coeffs[i])) return -1;
    }
    
    return degree;
}

int main(void) {
    const int MAX_DEGREE = 10;
    double coefficients[MAX_DEGREE + 1];
    
    int degree = read_coefficients(coefficients, MAX_DEGREE);
    if (degree < 0) {
        printf("Error: Invalid input\n");
        return 1;
    }
    
    printf("Enter initial guess: ");
    double guess;
    if (scanf("%lf", &guess) != 1 || !isfinite(guess)) {
        printf("Error: Invalid guess\n");
        return 1;
    }
    
    double root = newton_raphson(coefficients, degree, guess);
    
    if (!isfinite(root)) {
        printf("Error: Could not find root\n");
        return 1;
    }
    
    printf("Approximate root: %.10f\n", root);
    printf("Function value at root: %.10f\n", compute_polynomial(root, coefficients, degree));
    
    return 0;
}