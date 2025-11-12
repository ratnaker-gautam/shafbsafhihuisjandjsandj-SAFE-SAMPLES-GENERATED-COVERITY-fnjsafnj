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
        result += coeffs[i] * term;
        term *= x;
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
        result += i * coeffs[i] * term;
        term *= x;
    }
    return result;
}

int find_root_newton(double *root, const double *coeffs, int degree, double initial_guess) {
    if (root == NULL || coeffs == NULL || degree < 1) {
        return -1;
    }
    double x = initial_guess;
    for (int iter = 0; iter < MAX_ITERATIONS; iter++) {
        double fx = compute_polynomial(x, coeffs, degree);
        double dfx = compute_derivative(x, coeffs, degree);
        if (fabs(dfx) < EPSILON) {
            return -2;
        }
        double delta = fx / dfx;
        if (fabs(delta) < EPSILON) {
            *root = x;
            return 0;
        }
        x -= delta;
        if (!isfinite(x)) {
            return -3;
        }
    }
    return -4;
}

int read_polynomial_coeffs(double **coeffs, int *degree) {
    if (coeffs == NULL || degree == NULL) {
        return -1;
    }
    printf("Enter polynomial degree (1-10): ");
    if (scanf("%d", degree) != 1) {
        return -2;
    }
    if (*degree < 1 || *degree > 10) {
        return -3;
    }
    *coeffs = malloc((*degree + 1) * sizeof(double));
    if (*coeffs == NULL) {
        return -4;
    }
    printf("Enter %d coefficients from constant to highest degree:\n", *degree + 1);
    for (int i = 0; i <= *degree; i++) {
        if (scanf("%lf", &(*coeffs)[i]) != 1) {
            free(*coeffs);
            return -5;
        }
    }
    return 0;
}

int main(void) {
    double *coeffs = NULL;
    int degree = 0;
    int result = read_polynomial_coeffs(&coeffs, &degree);
    if (result != 0) {
        printf("Error reading polynomial coefficients.\n");
        return 1;
    }
    double initial_guess = 0.0;
    printf("Enter initial guess for root: ");
    if (scanf("%lf", &initial_guess) != 1) {
        free(coeffs);
        printf("Invalid initial guess.\n");
        return 1;
    }
    double root = 0.0;
    result = find_root_newton(&root, coeffs, degree, initial_guess);
    if (result == 0) {
        printf("Found root: %.10f\n", root);
        double verification = compute_polynomial(root, coeffs, degree);
        printf("Polynomial value at root: %.2e\n", verification);
    } else {
        printf("Root finding failed with error code %d.\n", result);
    }
    free(coeffs);
    return 0;
}