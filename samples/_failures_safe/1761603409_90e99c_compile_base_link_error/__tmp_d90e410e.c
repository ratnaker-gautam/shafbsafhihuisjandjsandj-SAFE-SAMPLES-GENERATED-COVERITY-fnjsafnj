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

double compute_series_sum(double x, int n_terms) {
    if (n_terms <= 0 || n_terms > MAX_ITERATIONS) {
        return NAN;
    }
    double sum = 0.0;
    double term = 1.0;
    for (int i = 1; i <= n_terms; i++) {
        term *= x / (double)i;
        sum += term;
        if (!isfinite(sum)) {
            return NAN;
        }
    }
    return sum;
}

double compute_function(double x) {
    if (x < MIN_INPUT || x > MAX_INPUT) {
        return NAN;
    }
    double result = 0.0;
    for (int i = 1; i <= MAX_ITERATIONS; i++) {
        double term = safe_pow(-1.0, i + 1) / safe_pow(i, x);
        if (!isfinite(term)) {
            break;
        }
        result += term;
        if (fabs(term) < EPSILON) {
            break;
        }
    }
    return result;
}

int main(void) {
    double input_value;
    printf("Enter a numeric value between %g and %g: ", MIN_INPUT, MAX_INPUT);
    if (scanf("%lf", &input_value) != 1) {
        fprintf(stderr, "Error: Invalid input format\n");
        return EXIT_FAILURE;
    }
    if (input_value < MIN_INPUT || input_value > MAX_INPUT) {
        fprintf(stderr, "Error: Input out of valid range\n");
        return EXIT_FAILURE;
    }
    double series_result = compute_series_sum(input_value, 50);
    if (!isfinite(series_result)) {
        fprintf(stderr, "Error: Series computation failed\n");
        return EXIT_FAILURE;
    }
    double function_result = compute_function(input_value);
    if (!isfinite(function_result)) {
        fprintf(stderr, "Error: Function computation failed\n");
        return EXIT_FAILURE;
    }
    double final_result = series_result * function_result;
    if (!isfinite(final_result)) {
        fprintf(stderr, "Error: Final computation failed\n");
        return EXIT_FAILURE;
    }
    printf("Computed result: %.10f\n", final_result);
    return EXIT_SUCCESS;
}