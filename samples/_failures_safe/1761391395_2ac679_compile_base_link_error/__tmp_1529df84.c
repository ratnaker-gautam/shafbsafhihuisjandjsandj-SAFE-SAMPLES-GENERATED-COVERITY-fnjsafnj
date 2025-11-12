//DeepSeek-V3 V2.5 Category: Safe ; Style: macros ; Variation: numeric_computation
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <limits.h>

#define MAX_ITERATIONS 1000
#define EPSILON 1e-10
#define MAX_INPUT 1000000.0
#define MIN_INPUT 1e-10

double compute_series_sum(double x, int n_terms) {
    if (n_terms <= 0 || n_terms > MAX_ITERATIONS) {
        return NAN;
    }
    double sum = 0.0;
    double term = 1.0;
    for (int i = 1; i <= n_terms; i++) {
        term *= x / i;
        sum += term;
    }
    return sum;
}

double compute_continued_fraction(double x, int depth) {
    if (depth <= 0 || depth > MAX_ITERATIONS) {
        return NAN;
    }
    double result = x + depth;
    for (int i = depth - 1; i >= 1; i--) {
        if (fabs(result) < EPSILON) {
            return NAN;
        }
        result = x + (double)i / result;
    }
    return result;
}

int main(void) {
    double input_value;
    int num_terms;
    
    printf("Enter a positive number (1e-10 to 1e6): ");
    if (scanf("%lf", &input_value) != 1) {
        fprintf(stderr, "Error: Invalid input format\n");
        return EXIT_FAILURE;
    }
    
    if (input_value < MIN_INPUT || input_value > MAX_INPUT) {
        fprintf(stderr, "Error: Input out of valid range\n");
        return EXIT_FAILURE;
    }
    
    printf("Enter number of terms (1 to %d): ", MAX_ITERATIONS);
    if (scanf("%d", &num_terms) != 1) {
        fprintf(stderr, "Error: Invalid input format\n");
        return EXIT_FAILURE;
    }
    
    double series_result = compute_series_sum(input_value, num_terms);
    if (isnan(series_result)) {
        fprintf(stderr, "Error: Invalid parameters for series computation\n");
        return EXIT_FAILURE;
    }
    
    double fraction_result = compute_continued_fraction(input_value, num_terms);
    if (isnan(fraction_result)) {
        fprintf(stderr, "Error: Invalid parameters for continued fraction\n");
        return EXIT_FAILURE;
    }
    
    double math_lib_result = exp(input_value);
    if (isinf(math_lib_result) || isnan(math_lib_result)) {
        fprintf(stderr, "Error: Math library computation failed\n");
        return EXIT_FAILURE;
    }
    
    printf("Series approximation: %.10f\n", series_result);
    printf("Continued fraction: %.10f\n", fraction_result);
    printf("Math library exp(x): %.10f\n", math_lib_result);
    printf("Series error: %.10f\n", fabs(series_result - math_lib_result));
    printf("Fraction error: %.10f\n", fabs(fraction_result - math_lib_result));
    
    return EXIT_SUCCESS;
}