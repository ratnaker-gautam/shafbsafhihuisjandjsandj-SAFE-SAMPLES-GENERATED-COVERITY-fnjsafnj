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
    return x * x - 2.0;
}

double compute_derivative(double x) {
    return 2.0 * x;
}

int main(void) {
    double initial_guess;
    double current;
    double next;
    double error;
    int iterations;
    
    printf("Enter initial guess for square root of 2: ");
    if (scanf("%lf", &initial_guess) != 1) {
        fprintf(stderr, "Error: Invalid input format\n");
        return EXIT_FAILURE;
    }
    
    if (!VALIDATE_INPUT(initial_guess)) {
        fprintf(stderr, "Error: Input out of valid range [%g, %g]\n", MIN_INPUT, MAX_INPUT);
        return EXIT_FAILURE;
    }
    
    if (ABS(initial_guess) < EPSILON) {
        fprintf(stderr, "Error: Initial guess too close to zero\n");
        return EXIT_FAILURE;
    }
    
    current = initial_guess;
    iterations = 0;
    
    while (iterations < MAX_ITERATIONS) {
        double f_val = compute_function(current);
        double f_deriv = compute_derivative(current);
        
        if (ABS(f_deriv) < EPSILON) {
            fprintf(stderr, "Error: Derivative too small, cannot continue\n");
            return EXIT_FAILURE;
        }
        
        next = current - f_val / f_deriv;
        error = ABS(next - current);
        
        if (error < EPSILON) {
            break;
        }
        
        current = next;
        iterations++;
    }
    
    if (iterations >= MAX_ITERATIONS) {
        fprintf(stderr, "Error: Maximum iterations reached\n");
        return EXIT_FAILURE;
    }
    
    printf("Approximated square root of 2: %.10f\n", current);
    printf("Iterations required: %d\n", iterations);
    printf("Verification: %.10f^2 = %.10f\n", current, current * current);
    
    return EXIT_SUCCESS;
}