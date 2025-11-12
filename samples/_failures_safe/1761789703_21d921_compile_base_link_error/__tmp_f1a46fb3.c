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

double compute_continued_fraction(double x, int depth) {
    if (depth <= 0 || depth > MAX_ITERATIONS) {
        return NAN;
    }
    double result = 1.0;
    for (int i = depth; i >= 1; i--) {
        result = 1.0 + x / result;
        if (!isfinite(result)) {
            return NAN;
        }
    }
    return result;
}

int main(void) {
    double input_value;
    int num_terms;
    
    printf("Enter a numeric value (1e-10 to 1e6): ");
    if (scanf("%lf", &input_value) != 1) {
        fprintf(stderr, "Error: Invalid numeric input\n");
        return EXIT_FAILURE;
    }
    
    if (input_value < MIN_INPUT || input_value > MAX_INPUT) {
        fprintf(stderr, "Error: Input out of valid range\n");
        return EXIT_FAILURE;
    }
    
    printf("Enter number of terms (1 to %d): ", MAX_ITERATIONS);
    if (scanf("%d", &num_terms) != 1) {
        fprintf(stderr, "Error: Invalid term count\n");
        return EXIT_FAILURE;
    }
    
    double series_result = compute_series_sum(input_value, num_terms);
    if (isnan(series_result)) {
        fprintf(stderr, "Error: Series computation failed\n");
        return EXIT_FAILURE;
    }
    
    double fraction_result = compute_continued_fraction(input_value, num_terms);
    if (isnan(fraction_result)) {
        fprintf(stderr, "Error: Continued fraction computation failed\n");
        return EXIT_FAILURE;
    }
    
    double power_result = safe_pow(input_value, 2.5);
    if (isnan(power_result) || !isfinite(power_result)) {
        fprintf(stderr, "Error: Power computation failed\n");
        return EXIT_FAILURE;
    }
    
    printf("Series sum approximation: %.10f\n", series_result);
    printf("Continued fraction result: %.10f\n", fraction_result);
    printf("Power result (x^2.5): %.10f\n", power_result);
    printf("Combined metric: %.10f\n", series_result * fraction_result / power_result);
    
    return EXIT_SUCCESS;
}