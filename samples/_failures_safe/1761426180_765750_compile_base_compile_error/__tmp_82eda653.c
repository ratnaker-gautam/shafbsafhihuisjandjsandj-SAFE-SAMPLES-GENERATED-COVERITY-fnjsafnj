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
    
    double result = compute_series_sum(input_value, num_terms);
    if (isnan(result)) {
        fprintf(stderr, "Error: Computation failed - series diverged or invalid parameters\n");
        return EXIT_FAILURE;
    }
    
    double exp_approx = safe_pow(E_CONST, input_value);
    if (!isfinite(exp_approx)) {
        fprintf(stderr, "Error: Reference computation failed\n");
        return EXIT_FAILURE;
    }
    
    double error = fabs(result - exp_approx);
    printf("Series sum: %.15g\n", result);
    printf("e^x approximation: %.15g\n", exp_approx);
    printf("Absolute error: %.15g\n", error);
    
    return EXIT_SUCCESS;
}