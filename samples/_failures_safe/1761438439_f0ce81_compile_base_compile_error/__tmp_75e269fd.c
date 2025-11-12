//DeepSeek-V3 V2.5 Category: Safe ; Style: modular_functions ; Variation: numeric_computation
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <stdint.h>
#include <limits.h>
#include <float.h>

#define MAX_ITERATIONS 1000
#define EPSILON 1e-10

double compute_polynomial(double x, const double *coeffs, int degree) {
    if (degree < 0 || coeffs == NULL) {
        return NAN;
    }
    double result = 0.0;
    double term = 1.0;
    for (int i = 0; i <= degree; i++) {
        if (i > 0) {
            if (fabs(x) > 1.0 && term > DBL_MAX / fabs(x)) {
                return NAN;
            }
            term *= x;
        }
        if (fabs(coeffs[i]) > DBL_MAX - fabs(result)) {
            return NAN;
        }
        result += coeffs[i] * term;
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
        if (i > 1) {
            if (fabs(x) > 1.0 && term > DBL_MAX / fabs(x)) {
                return NAN;
            }
            term *= x;
        }
        double derivative_term = i * coeffs[i] * term;
        if (fabs(derivative_term) > DBL_MAX - fabs(result)) {
            return NAN;
        }
        result += derivative_term;
    }
    return result;
}

double find_root_newton(const double *coeffs, int degree, double initial_guess) {
    if (degree < 1 || coeffs == NULL || !isfinite(initial_guess)) {
        return NAN;
    }
    double x = initial_guess;
    for (int i = 0; i < MAX_ITERATIONS; i++) {
        double fx = compute_polynomial(x, coeffs, degree);
        if (!isfinite(fx)) {
            return NAN;
        }
        if (fabs(fx) < EPSILON) {
            return x;
        }
        double dfx = compute_derivative(x, coeffs, degree);
        if (!isfinite(dfx) || fabs(dfx) < EPSILON) {
            return NAN;
        }
        double delta = fx / dfx;
        if (!isfinite(delta)) {
            return NAN;
        }
        double x_new = x - delta;
        if (!isfinite(x_new)) {
            return NAN;
        }
        if (fabs(x_new - x) < EPSILON) {
            return x_new;
        }
        x = x_new;
    }
    return NAN;
}

int read_polynomial_coeffs(double *coeffs, int max_degree) {
    if (coeffs == NULL || max_degree < 0) {
        return -1;
    }
    int degree = -1;
    while (degree < 0 || degree > max_degree) {
        printf("Enter polynomial degree (0-%d): ", max_degree);
        if (scanf("%d", &degree) != 1) {
            while (getchar() != '\n');
            printf("Invalid input. Please enter an integer.\n");
            degree = -1;
            continue;
        }
        if (degree < 0 || degree > max_degree) {
            printf("Degree must be between 0 and %d.\n", max_degree);
        }
    }
    printf("Enter %d coefficients (from constant term to highest degree):\n", degree + 1);
    for (int i = 0; i <= degree; i++) {
        printf("Coefficient for x^%d: ", i);
        if (scanf("%lf", &coeffs[i]) != 1) {
            while (getchar() != '\n');
            printf("Invalid input. Please enter a number.\n");
            i--;
            continue;
        }
        if (!isfinite(coeffs[i])) {
            printf("Coefficient must be a finite number.\n");
            i--;
            continue;
        }
    }
    return degree;
}

int main(void) {
    const int max_degree = 10;
    double coeffs[max_degree + 1];
    int degree = read_polynomial_coeffs(coeffs, max_degree);
    if (degree < 0) {
        return EXIT_FAILURE;
    }
    double initial_guess;
    printf("Enter initial guess for root finding: ");
    if (scanf("%lf", &initial_guess) != 1) {
        printf("Invalid initial guess.\n");
        return EXIT_FAILURE;
    }
    if (!isfinite(initial_guess)) {
        printf("Initial guess must be finite.\n");
        return EXIT_FAILURE;
    }
    double root = find_root_newton(coeffs, degree, initial_guess);
    if (!isfinite(root)) {
        printf("Could not find a root with the given parameters.\n");
    } else {
        printf("Found root at x = %.10f\n", root);
        double verification = compute_polynomial(root, coeffs, degree);
        printf("Polynomial value at root: %.10f