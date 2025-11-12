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
        if (isinf(term) || isnan(term)) {
            return NAN;
        }
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
        if (isinf(term) || isnan(term)) {
            return NAN;
        }
        result += i * coeffs[i] * term;
        term *= x;
    }
    return result;
}

int find_root_newton(const double *coeffs, int degree, double initial_guess, double *root) {
    if (degree < 1 || coeffs == NULL || root == NULL) {
        return -1;
    }
    double x = initial_guess;
    for (int iter = 0; iter < MAX_ITERATIONS; iter++) {
        double fx = compute_polynomial(x, coeffs, degree);
        double dfx = compute_derivative(x, coeffs, degree);
        if (isnan(fx) || isnan(dfx) || fabs(dfx) < EPSILON) {
            return -1;
        }
        double delta = fx / dfx;
        if (fabs(delta) < EPSILON) {
            *root = x;
            return 0;
        }
        x -= delta;
        if (isinf(x) || isnan(x)) {
            return -1;
        }
    }
    return -1;
}

int read_polynomial(double *coeffs, int max_degree, int *actual_degree) {
    if (coeffs == NULL || actual_degree == NULL || max_degree < 0) {
        return -1;
    }
    printf("Enter polynomial degree (0-%d): ", max_degree);
    if (scanf("%d", actual_degree) != 1) {
        return -1;
    }
    if (*actual_degree < 0 || *actual_degree > max_degree) {
        return -1;
    }
    printf("Enter %d coefficients from constant to highest degree:\n", *actual_degree + 1);
    for (int i = 0; i <= *actual_degree; i++) {
        if (scanf("%lf", &coeffs[i]) != 1) {
            return -1;
        }
    }
    return 0;
}

int main(void) {
    const int max_degree = 10;
    double coefficients[max_degree + 1];
    int degree;
    double initial_guess;
    double root;

    if (read_polynomial(coefficients, max_degree, &degree) != 0) {
        fprintf(stderr, "Error reading polynomial.\n");
        return EXIT_FAILURE;
    }

    printf("Enter initial guess for root: ");
    if (scanf("%lf", &initial_guess) != 1) {
        fprintf(stderr, "Error reading initial guess.\n");
        return EXIT_FAILURE;
    }

    if (find_root_newton(coefficients, degree, initial_guess, &root) == 0) {
        printf("Found root: %.10f\n", root);
        printf("Polynomial value at root: %.10f\n", compute_polynomial(root, coefficients, degree));
    } else {
        fprintf(stderr, "Failed to find root.\n");
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}