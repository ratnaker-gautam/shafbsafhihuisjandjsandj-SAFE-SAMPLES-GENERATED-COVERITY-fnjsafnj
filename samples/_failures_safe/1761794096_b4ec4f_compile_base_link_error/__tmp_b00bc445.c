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
        if (i == depth) {
            result = 1.0 + x / (double)i;
        } else {
            result = 1.0 + x / ((double)i - result);
        }
        if (!isfinite(result)) {
            return NAN;
        }
    }
    return 1.0 / result;
}

int main() {
    double input_value;
    int num_terms;
    
    printf("Enter a numeric value (1e-10 to 1000000): ");
    if (scanf("%lf", &input_value) != 1) {
        fprintf(stderr, "Error: Invalid numeric input\n");
        return EXIT_FAILURE;
    }
    
    if (input_value < MIN_INPUT || input_value > MAX_INPUT) {
        fprintf(stderr, "Error: Input out of valid range\n");
        return EXIT_FAILURE;
    }
    
    printf("Enter number of terms (1 to 1000): ");
    if (scanf("%d", &num_terms) != 1) {
        fprintf(stderr, "Error: Invalid term count\n");
        return EXIT_FAILURE;
    }
    
    double series_result = compute_series_sum(input_value, num_terms);
    double fraction_result = compute_continued_fraction(input_value, num_terms);
    double power_result = safe_pow(input_value, 2.5);
    
    if (!isfinite(series_result) || !isfinite(fraction_result) || !isfinite(power_result)) {
        fprintf(stderr, "Error: Computation resulted in invalid value\n");
        return EXIT_FAILURE;
    }
    
    printf("Series approximation: %.10f\n", series_result);
    printf("Continued fraction: %.10f\n", fraction_result);
    printf("Power function: %.10f\n", power_result);
    
    double combined_result = (series_result + fraction_result) / power_result;
    if (!isfinite(combined_result)) {
        fprintf(stderr, "Error: Combined result is invalid\n");
        return EXIT_FAILURE;
    }
    
    printf("Combined metric: %.10f\n", combined_result);
    
    return EXIT_SUCCESS;
}