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
#define E_CONST 2.71828182845904523536

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
        term *= x / i;
        if (!isfinite(term)) {
            return NAN;
        }
        sum += term;
        if (!isfinite(sum)) {
            return NAN;
        }
    }
    return sum;
}

int main(void) {
    double input_value;
    int num_terms;
    
    printf("Enter a value for x (between %.10g and %.10g): ", MIN_INPUT, MAX_INPUT);
    if (scanf("%lf", &input_value) != 1) {
        fprintf(stderr, "Error: Invalid input for x\n");
        return EXIT_FAILURE;
    }
    
    if (input_value < MIN_INPUT || input_value > MAX_INPUT) {
        fprintf(stderr, "Error: x out of valid range\n");
        return EXIT_FAILURE;
    }
    
    printf("Enter number of terms (1 to %d): ", MAX_ITERATIONS);
    if (scanf("%d", &num_terms) != 1) {
        fprintf(stderr, "Error: Invalid input for number of terms\n");
        return EXIT_FAILURE;
    }
    
    if (num_terms < 1 || num_terms > MAX_ITERATIONS) {
        fprintf(stderr, "Error: Number of terms out of valid range\n");
        return EXIT_FAILURE;
    }
    
    double series_result = compute_series_sum(input_value, num_terms);
    if (isnan(series_result)) {
        fprintf(stderr, "Error: Series computation failed\n");
        return EXIT_FAILURE;
    }
    
    double exp_result = safe_pow(E_CONST, input_value);
    if (isnan(exp_result) || !isfinite(exp_result)) {
        fprintf(stderr, "Error: Exponential computation failed\n");
        return EXIT_FAILURE;
    }
    
    double difference = fabs(series_result - exp_result);
    double relative_error = difference / fabs(exp_result);
    
    printf("Series approximation: %.15g\n", series_result);
    printf("Exponential function: %.15g\n", exp_result);
    printf("Absolute difference: %.15g\n", difference);
    printf("Relative error: %.15g\n", relative_error);
    
    if (relative_error < EPSILON) {
        printf("Approximation is within tolerance\n");
    } else {
        printf("Approximation exceeds tolerance\n");
    }
    
    return EXIT_SUCCESS;
}