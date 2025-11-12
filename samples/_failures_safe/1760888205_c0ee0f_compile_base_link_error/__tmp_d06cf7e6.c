//DeepSeek-V3 SAFE v1.4 Category: Safe ; Style: macros ; Variation: numeric_computation
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <stdint.h>
#include <limits.h>

#define MAX_ITERATIONS 1000
#define EPSILON 1e-10
#define MAX_INPUT 1000000.0
#define MIN_INPUT 1e-10

double compute_series(double x, int max_iter) {
    if (max_iter <= 0) return 0.0;
    double sum = 0.0;
    double term = x;
    for (int n = 1; n <= max_iter; n++) {
        if (fabs(term) < EPSILON) break;
        sum += term;
        if (n == max_iter) break;
        term = term * (-x * x) / ((2.0 * n) * (2.0 * n + 1.0));
        if (!isfinite(term)) break;
    }
    return sum;
}

int main(void) {
    double input_value;
    char buffer[256];
    
    printf("Enter a value for x (between %.10g and %.10g): ", MIN_INPUT, MAX_INPUT);
    
    if (fgets(buffer, sizeof(buffer), stdin) == NULL) {
        fprintf(stderr, "Error reading input\n");
        return EXIT_FAILURE;
    }
    
    char *endptr;
    input_value = strtod(buffer, &endptr);
    if (endptr == buffer || *endptr != '\n') {
        fprintf(stderr, "Invalid input format\n");
        return EXIT_FAILURE;
    }
    
    if (input_value < MIN_INPUT || input_value > MAX_INPUT) {
        fprintf(stderr, "Input out of valid range\n");
        return EXIT_FAILURE;
    }
    
    if (!isfinite(input_value)) {
        fprintf(stderr, "Input is not a finite number\n");
        return EXIT_FAILURE;
    }
    
    double result = compute_series(input_value, MAX_ITERATIONS);
    double reference = sin(input_value);
    
    if (!isfinite(result)) {
        fprintf(stderr, "Computation resulted in non-finite value\n");
        return EXIT_FAILURE;
    }
    
    printf("Series approximation: %.15g\n", result);
    printf("Reference sin(x): %.15g\n", reference);
    printf("Absolute error: %.15g\n", fabs(result - reference));
    
    return EXIT_SUCCESS;
}