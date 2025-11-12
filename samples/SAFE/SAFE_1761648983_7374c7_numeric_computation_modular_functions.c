//DeepSeek-V3 V2.5 Category: Safe ; Style: modular_functions ; Variation: numeric_computation
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <stdint.h>
#include <limits.h>

#define MAX_ITERATIONS 1000
#define EPSILON 1e-10

double compute_polynomial(double x, const double *coeffs, int degree) {
    if (degree < 0 || coeffs == NULL) {
        return NAN;
    }
    double result = 0.0;
    double term = 1.0;
    for (int i = 0; i <= degree; i++) {
        if (isfinite(term) && isfinite(coeffs[i])) {
            result += coeffs[i] * term;
            term *= x;
        } else {
            return NAN;
        }
    }
    return result;
}

double compute_derivative(double x, const double *coeffs, int degree) {
    if (degree < 1 || coeffs == NULL) {
        return NAN;
    }
    double result = 0.0;
    double term = 1.0;
    for (int i = 1; i <= degree; i++) {
        if (isfinite(term) && isfinite(coeffs[i])) {
            result += i * coeffs[i] * term;
            term *= x;
        } else {
            return NAN;
        }
    }
    return result;
}

double newton_raphson(const double *coeffs, int degree, double initial_guess) {
    if (degree < 1 || coeffs == NULL || !isfinite(initial_guess)) {
        return NAN;
    }
    double x = initial_guess;
    for (int i = 0; i < MAX_ITERATIONS; i++) {
        double fx = compute_polynomial(x, coeffs, degree);
        double dfx = compute_derivative(x, coeffs, degree);
        if (!isfinite(fx) || !isfinite(dfx) || fabs(dfx) < EPSILON) {
            return NAN;
        }
        double delta = fx / dfx;
        if (!isfinite(delta)) {
            return NAN;
        }
        x -= delta;
        if (fabs(delta) < EPSILON) {
            return x;
        }
    }
    return NAN;
}

int read_coefficients(double *coeffs, int max_degree) {
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
    printf("Enter %d coefficients from constant to highest term:\n", degree + 1);
    for (int i = 0; i <= degree; i++) {
        if (scanf("%lf", &coeffs[i]) != 1) {
            return -1;
        }
        if (!isfinite(coeffs[i])) {
            return -1;
        }
    }
    return degree;
}

int main(void) {
    const int max_degree = 10;
    double coeffs[max_degree + 1];
    int degree = read_coefficients(coeffs, max_degree);
    if (degree < 0) {
        printf("Invalid input.\n");
        return EXIT_FAILURE;
    }
    double initial_guess;
    printf("Enter initial guess: ");
    if (scanf("%lf", &initial_guess) != 1 || !isfinite(initial_guess)) {
        printf("Invalid initial guess.\n");
        return EXIT_FAILURE;
    }
    double root = newton_raphson(coeffs, degree, initial_guess);
    if (isfinite(root)) {
        printf("Approximate root: %.10f\n", root);
        printf("Polynomial value at root: %.10f\n", compute_polynomial(root, coeffs, degree));
    } else {
        printf("Failed to converge or invalid input.\n");
        return EXIT_FAILURE;
    }
    return EXIT_SUCCESS;
}