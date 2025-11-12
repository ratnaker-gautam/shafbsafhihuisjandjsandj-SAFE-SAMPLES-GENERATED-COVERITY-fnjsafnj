//DeepSeek-V3 V2.5 Category: Safe ; Style: modular_functions ; Variation: numeric_computation
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <stdint.h>
#include <limits.h>

#define MAX_ITERATIONS 1000
#define EPSILON 1e-10

double compute_polynomial(double x, double* coeffs, int degree) {
    if (coeffs == NULL || degree < 0) {
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

double compute_derivative(double x, double* coeffs, int degree) {
    if (coeffs == NULL || degree < 1) {
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

double find_root_newton(double* coeffs, int degree, double initial_guess) {
    if (coeffs == NULL || degree < 1) {
        return NAN;
    }
    double x = initial_guess;
    for (int i = 0; i < MAX_ITERATIONS; i++) {
        double fx = compute_polynomial(x, coeffs, degree);
        if (fabs(fx) < EPSILON) {
            return x;
        }
        double fpx = compute_derivative(x, coeffs, degree);
        if (fabs(fpx) < EPSILON) {
            return NAN;
        }
        double delta = fx / fpx;
        if (isinf(delta) || isnan(delta)) {
            return NAN;
        }
        x -= delta;
        if (isinf(x) || isnan(x)) {
            return NAN;
        }
    }
    return NAN;
}

int read_polynomial(double** coeffs, int* degree) {
    printf("Enter polynomial degree (1-10): ");
    if (scanf("%d", degree) != 1) {
        return 0;
    }
    if (*degree < 1 || *degree > 10) {
        return 0;
    }
    *coeffs = malloc((*degree + 1) * sizeof(double));
    if (*coeffs == NULL) {
        return 0;
    }
    printf("Enter %d coefficients from constant to highest degree:\n", *degree + 1);
    for (int i = 0; i <= *degree; i++) {
        if (scanf("%lf", &(*coeffs)[i]) != 1) {
            free(*coeffs);
            return 0;
        }
    }
    return 1;
}

int main(void) {
    double* coeffs = NULL;
    int degree = 0;
    
    if (!read_polynomial(&coeffs, &degree)) {
        printf("Error: Invalid polynomial input\n");
        return 1;
    }
    
    double initial_guess;
    printf("Enter initial guess for root finding: ");
    if (scanf("%lf", &initial_guess) != 1) {
        printf("Error: Invalid initial guess\n");
        free(coeffs);
        return 1;
    }
    
    double root = find_root_newton(coeffs, degree, initial_guess);
    
    if (isnan(root)) {
        printf("Failed to find root within %d iterations\n", MAX_ITERATIONS);
    } else {
        printf("Approximate root: %.10f\n", root);
        double verification = compute_polynomial(root, coeffs, degree);
        printf("Polynomial value at root: %.10f\n", verification);
    }
    
    free(coeffs);
    return 0;
}