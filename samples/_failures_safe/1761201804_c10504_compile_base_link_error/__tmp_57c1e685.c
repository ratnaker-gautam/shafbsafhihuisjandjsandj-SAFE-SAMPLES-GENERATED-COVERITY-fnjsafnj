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
        sum += term;
    }
    return sum;
}

double compute_function(double x) {
    if (x < MIN_INPUT || x > MAX_INPUT) {
        return NAN;
    }
    double result = 0.0;
    for (int i = 1; i <= MAX_ITERATIONS; i++) {
        double term = safe_pow(-1.0, i + 1) / (i * safe_pow(x, i));
        if (isnan(term) || isinf(term)) {
            break;
        }
        result += term;
        if (fabs(term) < EPSILON) {
            break;
        }
    }
    return result;
}

int main() {
    double input_value;
    int num_terms;
    
    printf("Enter a positive number between %e and %e: ", MIN_INPUT, MAX_INPUT);
    if (scanf("%lf", &input_value) != 1) {
        fprintf(stderr, "Error: Invalid input format\n");
        return EXIT_FAILURE;
    }
    
    if (input_value < MIN_INPUT || input_value > MAX_INPUT) {
        fprintf(stderr, "Error: Input out of valid range\n");
        return EXIT_FAILURE;
    }
    
    printf("Enter number of series terms (1-%d): ", MAX_ITERATIONS);
    if (scanf("%d", &num_terms) != 1) {
        fprintf(stderr, "Error: Invalid input format\n");
        return EXIT_FAILURE;
    }
    
    double series_result = compute_series_sum(input_value, num_terms);
    if (isnan(series_result)) {
        fprintf(stderr, "Error: Invalid series computation\n");
        return EXIT_FAILURE;
    }
    
    double function_result = compute_function(input_value);
    if (isnan(function_result)) {
        fprintf(stderr, "Error: Invalid function computation\n");
        return EXIT_FAILURE;
    }
    
    printf("Series approximation result: %.10f\n", series_result);
    printf("Function computation result: %.10f\n", function_result);
    printf("Difference: %.10f\n", fabs(series_result - function_result));
    
    return EXIT_SUCCESS;
}