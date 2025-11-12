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

double newton_raphson(double* coeffs, int degree, double initial_guess) {
    if (coeffs == NULL || degree < 1) return NAN;
    double x = initial_guess;
    for (int i = 0; i < MAX_ITERATIONS; i++) {
        double fx = compute_polynomial(x, coeffs, degree);
        double fpx = compute_derivative(x, coeffs, degree);
        if (fabs(fpx) < EPSILON) break;
        double delta = fx / fpx;
        if (fabs(delta) < EPSILON) break;
        x -= delta;
        if (x < -1e6 || x > 1e6) break;
    }
    return x;
}

int read_coefficients(double* coeffs, int max_degree) {
    if (coeffs == NULL || max_degree < 0) return -1;
    int degree;
    printf("Enter polynomial degree (0-%d): ", max_degree);
    if (scanf("%d", &degree) != 1) return -1;
    if (degree < 0 || degree > max_degree) return -1;
    printf("Enter %d coefficients from constant to highest degree: ", degree + 1);
    for (int i = 0; i <= degree; i++) {
        if (scanf("%lf", &coeffs[i]) != 1) return -1;
    }
    return degree;
}

int main(void) {
    const int max_degree = 10;
    double coefficients[max_degree + 1];
    int degree = read_coefficients(coefficients, max_degree);
    if (degree < 0) {
        printf("Invalid input.\n");
        return 1;
    }
    double guess;
    printf("Enter initial guess: ");
    if (scanf("%lf", &guess) != 1) {
        printf("Invalid guess.\n");
        return 1;
    }
    double root = newton_raphson(coefficients, degree, guess);
    if (isnan(root)) {
        printf("Failed to find root.\n");
    } else {
        printf("Approximate root: %.10f\n", root);
        printf("Polynomial value at root: %.10f\n", compute_polynomial(root, coefficients, degree));
    }
    return 0;
}