//DeepSeek-V3 SAFE v1.4 Category: Safe ; Style: macros ; Variation: numeric_computation
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <stdint.h>
#include <limits.h>

#define MAX_ITERATIONS 1000
#define EPSILON 1e-10
#define MAX_INPUT 1000000.0
#define MIN_INPUT 0.0001

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
        if (!isfinite(term)) {
            return NAN;
        }
        result += term;
        if (!isfinite(result)) {
            return NAN;
        }
    }
    return result;
}

int main(void) {
    double input_value;
    int terms;
    
    printf("Enter a value between %g and %g: ", MIN_INPUT, MAX_INPUT);
    if (scanf("%lf", &input_value) != 1) {
        fprintf(stderr, "Error: Invalid input format\n");
        return EXIT_FAILURE;
    }
    
    if (input_value < MIN_INPUT || input_value > MAX_INPUT) {
        fprintf(stderr, "Error: Input out of valid range\n");
        return EXIT_FAILURE;
    }
    
    printf("Enter number of terms (1-%d): ", MAX_ITERATIONS);
    if (scanf("%d", &terms) != 1) {
        fprintf(stderr, "Error: Invalid term count\n");
        return EXIT_FAILURE;
    }
    
    double series_result = compute_series(input_value, terms);
    if (isnan(series_result)) {
        fprintf(stderr, "Error: Series computation failed\n");
        return EXIT_FAILURE;
    }
    
    double power_result = safe_pow(input_value, 2.5);
    if (isnan(power_result) || !isfinite(power_result)) {
        fprintf(stderr, "Error: Power computation failed\n");
        return EXIT_FAILURE;
    }
    
    double combined = series_result * power_result;
    if (!isfinite(combined)) {
        fprintf(stderr, "Error: Combined result is not finite\n");
        return EXIT_FAILURE;
    }
    
    printf("Series approximation: %.10g\n", series_result);
    printf("Power result: %.10g\n", power_result);
    printf("Combined result: %.10g\n", combined);
    
    return EXIT_SUCCESS;
}