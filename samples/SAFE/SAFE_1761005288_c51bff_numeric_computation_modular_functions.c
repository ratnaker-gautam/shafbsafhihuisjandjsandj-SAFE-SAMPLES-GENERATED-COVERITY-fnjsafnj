//DeepSeek-V3 SAFE v1.4 Category: Safe ; Style: modular_functions ; Variation: numeric_computation
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <stdint.h>
#include <limits.h>

#define MAX_ITERATIONS 1000
#define EPSILON 1e-10

double compute_polynomial(double x, const double* coeffs, int degree) {
    if (degree < 0 || coeffs == NULL) {
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

double compute_derivative(double x, const double* coeffs, int degree) {
    if (degree < 1 || coeffs == NULL) {
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

double find_root_newton(const double* coeffs, int degree, double initial_guess) {
    if (degree < 1 || coeffs == NULL || isnan(initial_guess) || isinf(initial_guess)) {
        return NAN;
    }
    double x = initial_guess;
    for (int iter = 0; iter < MAX_ITERATIONS; iter++) {
        double fx = compute_polynomial(x, coeffs, degree);
        double dfx = compute_derivative(x, coeffs, degree);
        if (isnan(fx) || isnan(dfx) || fabs(dfx) < EPSILON) {
            return NAN;
        }
        double delta = fx / dfx;
        if (isnan(delta) || isinf(delta)) {
            return NAN;
        }
        x -= delta;
        if (fabs(delta) < EPSILON) {
            return x;
        }
    }
    return NAN;
}

int read_polynomial_coeffs(double* coeffs, int max_degree) {
    if (coeffs == NULL || max_degree < 0) {
        return -1;
    }
    int degree;
    printf("Enter polynomial degree (0-%d): ", max_degree);
    if (scanf("%d", &degree) != 1) {
        return -1;
    }
    if (degree < 0 || degree > max_degree) {
        return -1;
    }
    printf("Enter %d coefficients from constant to highest term: ", degree + 1);
    for (int i = 0; i <= degree; i++) {
        if (scanf("%lf", &coeffs[i]) != 1) {
            return -1;
        }
    }
    return degree;
}

int main(void) {
    const int max_degree = 10;
    double coeffs[max_degree + 1];
    int degree = read_polynomial_coeffs(coeffs, max_degree);
    if (degree < 0) {
        printf("Invalid input.\n");
        return 1;
    }
    double initial_guess;
    printf("Enter initial guess: ");
    if (scanf("%lf", &initial_guess) != 1) {
        printf("Invalid input.\n");
        return 1;
    }
    double root = find_root_newton(coeffs, degree, initial_guess);
    if (isnan(root)) {
        printf("Failed to find root.\n");
        return 1;
    }
    printf("Found root: %.10f\n", root);
    printf("Polynomial value at root: %.10f\n", compute_polynomial(root, coeffs, degree));
    return 0;
}