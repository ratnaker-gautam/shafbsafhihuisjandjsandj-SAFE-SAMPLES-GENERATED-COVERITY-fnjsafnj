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
#define IS_VALID(x) ((x) >= MIN_INPUT && (x) <= MAX_INPUT)

double compute_function(double x) {
    return x * x * x - 2 * x * x - 5 * x + 6;
}

double compute_derivative(double x) {
    return 3 * x * x - 4 * x - 5;
}

int validate_input(double x) {
    if (!IS_VALID(x)) {
        return 0;
    }
    if (isnan(x) || isinf(x)) {
        return 0;
    }
    return 1;
}

int main(void) {
    double initial_guess;
    double root;
    double f_val, f_deriv;
    int iterations = 0;
    
    printf("Enter initial guess for root finding: ");
    if (scanf("%lf", &initial_guess) != 1) {
        fprintf(stderr, "Error: Invalid input format\n");
        return EXIT_FAILURE;
    }
    
    if (!validate_input(initial_guess)) {
        fprintf(stderr, "Error: Input out of valid range\n");
        return EXIT_FAILURE;
    }
    
    root = initial_guess;
    
    while (iterations < MAX_ITERATIONS) {
        f_val = compute_function(root);
        f_deriv = compute_derivative(root);
        
        if (fabs(f_deriv) < EPSILON) {
            fprintf(stderr, "Error: Derivative too small\n");
            return EXIT_FAILURE;
        }
        
        double delta = f_val / f_deriv;
        
        if (!validate_input(root - delta)) {
            fprintf(stderr, "Error: Computation would exceed bounds\n");
            return EXIT_FAILURE;
        }
        
        root = root - delta;
        
        if (fabs(delta) < EPSILON) {
            break;
        }
        
        iterations++;
    }
    
    if (iterations >= MAX_ITERATIONS) {
        fprintf(stderr, "Error: Maximum iterations reached\n");
        return EXIT_FAILURE;
    }
    
    if (!validate_input(root)) {
        fprintf(stderr, "Error: Computed root out of bounds\n");
        return EXIT_FAILURE;
    }
    
    printf("Approximate root: %.10f\n", root);
    printf("Function value at root: %.10f\n", compute_function(root));
    printf("Iterations used: %d\n", iterations);
    
    return EXIT_SUCCESS;
}