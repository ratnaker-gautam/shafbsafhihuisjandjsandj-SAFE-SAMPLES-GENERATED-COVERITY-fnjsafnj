//DeepSeek-V3 V2.5 Category: Safe ; Style: macros ; Variation: numeric_computation
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <stdint.h>
#include <limits.h>
#include <float.h>

#define MAX_ITERATIONS 1000
#define EPSILON 1e-10
#define MAX_INPUT 1000000.0
#define MIN_INPUT -1000000.0
#define IS_VALID(x) ((x) >= MIN_INPUT && (x) <= MAX_INPUT)

double safe_power(double base, int exponent) {
    if (exponent == 0) return 1.0;
    if (base == 0.0) return 0.0;
    
    double result = 1.0;
    int abs_exp = exponent < 0 ? -exponent : exponent;
    
    for (int i = 0; i < abs_exp; i++) {
        if (result > DBL_MAX / fabs(base)) {
            return exponent > 0 ? DBL_MAX : 0.0;
        }
        result *= base;
    }
    
    return exponent < 0 ? 1.0 / result : result;
}

double compute_function(double x) {
    return safe_power(x, 3) - 2.0 * safe_power(x, 2) + x - 6.0;
}

double compute_derivative(double x) {
    return 3.0 * safe_power(x, 2) - 4.0 * x + 1.0;
}

int main(void) {
    double initial_guess;
    char buffer[256];
    
    printf("Enter initial guess for root finding (between %.1f and %.1f): ", MIN_INPUT, MAX_INPUT);
    
    if (fgets(buffer, sizeof(buffer), stdin) == NULL) {
        fprintf(stderr, "Error reading input\n");
        return EXIT_FAILURE;
    }
    
    char *endptr;
    initial_guess = strtod(buffer, &endptr);
    if (endptr == buffer || *endptr != '\n') {
        fprintf(stderr, "Invalid input format\n");
        return EXIT_FAILURE;
    }
    
    if (!IS_VALID(initial_guess)) {
        fprintf(stderr, "Input out of valid range\n");
        return EXIT_FAILURE;
    }
    
    double x = initial_guess;
    double prev_x;
    int iterations = 0;
    
    while (iterations < MAX_ITERATIONS) {
        double fx = compute_function(x);
        double fpx = compute_derivative(x);
        
        if (fabs(fpx) < EPSILON) {
            fprintf(stderr, "Derivative too small, cannot continue\n");
            return EXIT_FAILURE;
        }
        
        prev_x = x;
        x = x - fx / fpx;
        
        if (!IS_VALID(x)) {
            fprintf(stderr, "Computation resulted in out-of-range value\n");
            return EXIT_FAILURE;
        }
        
        if (fabs(x - prev_x) < EPSILON) {
            break;
        }
        
        iterations++;
    }
    
    if (iterations >= MAX_ITERATIONS) {
        fprintf(stderr, "Maximum iterations reached without convergence\n");
        return EXIT_FAILURE;
    }
    
    double final_value = compute_function(x);
    if (fabs(final_value) > 1.0) {
        fprintf(stderr, "Solution may not be accurate\n");
        return EXIT_FAILURE;
    }
    
    printf("Approximate root: %.10f\n", x);
    printf("Function value at root: %.10f\n", final_value);
    printf("Iterations used: %d\n", iterations);
    
    return EXIT_SUCCESS;
}