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
        if (isinf(sum) || isnan(sum)) {
            return NAN;
        }
    }
    return sum;
}

double validate_input_double(const char* prompt, double min_val, double max_val) {
    char buffer[256];
    double value;
    while (1) {
        printf("%s", prompt);
        if (fgets(buffer, sizeof(buffer), stdin) == NULL) {
            return NAN;
        }
        char* endptr;
        value = strtod(buffer, &endptr);
        if (endptr == buffer || *endptr != '\n') {
            printf("Invalid input. Please enter a valid number.\n");
            continue;
        }
        if (value < min_val || value > max_val) {
            printf("Value out of range. Must be between %g and %g.\n", min_val, max_val);
            continue;
        }
        break;
    }
    return value;
}

int validate_input_int(const char* prompt, int min_val, int max_val) {
    char buffer[256];
    long value;
    while (1) {
        printf("%s", prompt);
        if (fgets(buffer, sizeof(buffer), stdin) == NULL) {
            return -1;
        }
        char* endptr;
        value = strtol(buffer, &endptr, 10);
        if (endptr == buffer || *endptr != '\n') {
            printf("Invalid input. Please enter a valid integer.\n");
            continue;
        }
        if (value < min_val || value > max_val) {
            printf("Value out of range. Must be between %d and %d.\n", min_val, max_val);
            continue;
        }
        break;
    }
    return (int)value;
}

int main(void) {
    printf("Series Sum Calculator (e^x approximation)\n");
    printf("=========================================\n");
    
    double x = validate_input_double("Enter x value (1e-10 to 1000000): ", MIN_INPUT, MAX_INPUT);
    if (isnan(x)) {
        fprintf(stderr, "Input error.\n");
        return EXIT_FAILURE;
    }
    
    int n_terms = validate_input_int("Enter number of terms (1 to 1000): ", 1, MAX_ITERATIONS);
    if (n_terms == -1) {
        fprintf(stderr, "Input error.\n");
        return EXIT_FAILURE;
    }
    
    double result = compute_series_sum(x, n_terms);
    if (isnan(result)) {
        fprintf(stderr, "Computation error: series diverged or invalid parameters.\n");
        return EXIT_FAILURE;
    }
    
    printf("Series sum for e^%g with %d terms: %.15g\n", x, n_terms, result);
    printf("Library exp(%g) for comparison: %.15g\n", x, exp(x));
    printf("Absolute difference: %.15g\n", fabs(result - exp(x)));
    
    return EXIT_SUCCESS;
}