//DeepSeek-V3 V2.5 Category: Safe ; Style: macros ; Variation: numeric_computation
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <stdint.h>
#include <limits.h>

#define MAX_ITERATIONS 1000
#define TOLERANCE 1e-10
#define MAX_INPUT 1000000.0
#define MIN_INPUT 1e-10

double safe_sqrt(double x) {
    if (x < 0.0) {
        return NAN;
    }
    return sqrt(x);
}

double compute_series_sum(double x, int n_terms) {
    if (n_terms <= 0 || n_terms > MAX_ITERATIONS) {
        return NAN;
    }
    double sum = 0.0;
    double term = 1.0;
    for (int i = 0; i < n_terms; i++) {
        sum += term;
        if (i < n_terms - 1) {
            term *= x / (i + 1);
            if (!isfinite(term)) {
                return NAN;
            }
        }
    }
    return sum;
}

int main(void) {
    double input_value;
    int num_terms;
    
    printf("Enter a value for x (between %g and %g): ", MIN_INPUT, MAX_INPUT);
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
    
    double sqrt_result = safe_sqrt(input_value);
    if (isnan(sqrt_result)) {
        fprintf(stderr, "Error: Cannot compute square root of negative number\n");
        return EXIT_FAILURE;
    }
    
    double series_result = compute_series_sum(input_value, num_terms);
    if (isnan(series_result)) {
        fprintf(stderr, "Error: Series computation failed\n");
        return EXIT_FAILURE;
    }
    
    double final_result = sqrt_result * series_result;
    if (!isfinite(final_result)) {
        fprintf(stderr, "Error: Result is not finite\n");
        return EXIT_FAILURE;
    }
    
    printf("Square root of %.6f: %.6f\n", input_value, sqrt_result);
    printf("Series sum with %d terms: %.6f\n", num_terms, series_result);
    printf("Final result (sqrt * series): %.6f\n", final_result);
    
    return EXIT_SUCCESS;
}