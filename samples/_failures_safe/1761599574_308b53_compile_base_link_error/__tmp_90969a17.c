//DeepSeek-V3 V2.5 Category: Safe ; Style: macros ; Variation: numeric_computation
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <stdint.h>
#include <limits.h>

#define MAX_ITERATIONS 1000
#define EPSILON 1e-10
#define MAX_INPUT 1000000.0
#define MIN_INPUT 1e-10

double safe_pow(double base, double exponent) {
    if (base < 0.0 && fmod(exponent, 1.0) != 0.0) {
        return NAN;
    }
    if (base == 0.0 && exponent < 0.0) {
        return INFINITY;
    }
    return pow(base, exponent);
}

double compute_series(double x, int n_terms) {
    if (n_terms <= 0 || n_terms > MAX_ITERATIONS) {
        return NAN;
    }
    double result = 0.0;
    double term = 1.0;
    for (int i = 1; i <= n_terms; i++) {
        term *= x / (double)i;
        result += term;
        if (!isfinite(result)) {
            return NAN;
        }
    }
    return result;
}

double compute_function(double x) {
    if (x < 0.0 || x > MAX_INPUT) {
        return NAN;
    }
    double series_val = compute_series(x, 50);
    if (!isfinite(series_val)) {
        return NAN;
    }
    double root_val = safe_pow(series_val, 1.0 / 3.0);
    if (!isfinite(root_val)) {
        return NAN;
    }
    return root_val;
}

int main(void) {
    double input_value;
    printf("Enter a positive number (1e-10 to 1e6): ");
    if (scanf("%lf", &input_value) != 1) {
        fprintf(stderr, "Error: Invalid input format\n");
        return EXIT_FAILURE;
    }
    if (input_value < MIN_INPUT || input_value > MAX_INPUT) {
        fprintf(stderr, "Error: Input out of valid range\n");
        return EXIT_FAILURE;
    }
    double result = compute_function(input_value);
    if (!isfinite(result)) {
        fprintf(stderr, "Error: Computation failed\n");
        return EXIT_FAILURE;
    }
    printf("Computed value: %.10f\n", result);
    double reference = exp(input_value / 3.0);
    printf("Reference exp(x/3): %.10f\n", reference);
    printf("Absolute difference: %.10f\n", fabs(result - reference));
    return EXIT_SUCCESS;
}