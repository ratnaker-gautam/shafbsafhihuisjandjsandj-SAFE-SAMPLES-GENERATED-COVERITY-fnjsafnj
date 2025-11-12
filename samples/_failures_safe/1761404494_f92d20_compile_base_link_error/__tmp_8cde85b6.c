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
    int terms;
    
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
    if (scanf("%d", &terms) != 1) {
        fprintf(stderr, "Error: Invalid input for terms\n");
        return EXIT_FAILURE;
    }
    
    double series_result = compute_series_sum(input_value, terms);
    if (isnan(series_result)) {
        fprintf(stderr, "Error: Series computation failed\n");
        return EXIT_FAILURE;
    }
    
    double power_result = safe_pow(input_value, 2.5);
    if (!isfinite(power_result)) {
        fprintf(stderr, "Error: Power computation failed\n");
        return EXIT_FAILURE;
    }
    
    double final_result = series_result * power_result;
    if (!isfinite(final_result)) {
        fprintf(stderr, "Error: Final computation overflow\n");
        return EXIT_FAILURE;
    }
    
    printf("Series sum: %.10g\n", series_result);
    printf("Power result: %.10g\n", power_result);
    printf("Final result: %.10g\n", final_result);
    
    return EXIT_SUCCESS;
}