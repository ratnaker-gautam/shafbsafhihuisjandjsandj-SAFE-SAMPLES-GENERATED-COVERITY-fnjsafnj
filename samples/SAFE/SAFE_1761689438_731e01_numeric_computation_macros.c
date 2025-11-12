//DeepSeek-V3 V2.5 Category: Safe ; Style: macros ; Variation: numeric_computation
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <stdint.h>
#include <limits.h>

#define MAX_ITERATIONS 1000
#define EPSILON 1e-10
#define MAX_INPUT 1000000.0
#define MIN_INPUT -1000000.0

#define VALIDATE_INPUT(x) ((x) >= MIN_INPUT && (x) <= MAX_INPUT)
#define ABS(x) ((x) < 0 ? -(x) : (x))

double compute_function(double x) {
    return x * x * x - 2 * x * x - 5 * x + 6;
}

double compute_derivative(double x) {
    return 3 * x * x - 4 * x - 5;
}

int main(void) {
    double initial_guess;
    double x, x_new;
    int iterations;
    
    printf("Enter initial guess for root finding: ");
    if (scanf("%lf", &initial_guess) != 1) {
        fprintf(stderr, "Error: Invalid input format\n");
        return EXIT_FAILURE;
    }
    
    if (!VALIDATE_INPUT(initial_guess)) {
        fprintf(stderr, "Error: Input out of valid range [%g, %g]\n", MIN_INPUT, MAX_INPUT);
        return EXIT_FAILURE;
    }
    
    x = initial_guess;
    iterations = 0;
    
    while (iterations < MAX_ITERATIONS) {
        double fx = compute_function(x);
        double fpx = compute_derivative(x);
        
        if (ABS(fpx) < EPSILON) {
            fprintf(stderr, "Error: Derivative too small, method fails\n");
            return EXIT_FAILURE;
        }
        
        x_new = x - fx / fpx;
        
        if (!VALIDATE_INPUT(x_new)) {
            fprintf(stderr, "Error: Computation resulted in out-of-range value\n");
            return EXIT_FAILURE;
        }
        
        if (ABS(x_new - x) < EPSILON) {
            break;
        }
        
        x = x_new;
        iterations++;
    }
    
    if (iterations >= MAX_ITERATIONS) {
        fprintf(stderr, "Error: Maximum iterations reached without convergence\n");
        return EXIT_FAILURE;
    }
    
    double final_value = compute_function(x_new);
    
    if (ABS(final_value) > 0.1) {
        fprintf(stderr, "Warning: Solution may not be accurate, function value: %g\n", final_value);
    }
    
    printf("Approximate root: %.10g\n", x_new);
    printf("Function value at root: %.10g\n", final_value);
    printf("Iterations required: %d\n", iterations);
    
    return EXIT_SUCCESS;
}