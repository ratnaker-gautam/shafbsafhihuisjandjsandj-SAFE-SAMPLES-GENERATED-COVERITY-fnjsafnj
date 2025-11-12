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
#define SQUARE(x) ((x) * (x))
#define ABS(x) ((x) < 0 ? -(x) : (x))

double compute_function(double x) {
    return SQUARE(x) - 2.0;
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
        double f_current = compute_function(current);
        double f_prime_current = compute_derivative(current);
        
        if (ABS(f_prime_current) < EPSILON) {
            fprintf(stderr, "Error: Derivative too small, method fails\n");
            return EXIT_FAILURE;
        }
        
        next = current - f_current / f_prime_current;
        error = ABS(next - current);
        
        if (error < EPSILON) {
            break;
        }
        
        if (!VALIDATE_INPUT(next)) {
            fprintf(stderr, "Error: Computation diverged\n");
            return EXIT_FAILURE;
        }
        
        current = next;
        iterations++;
    }
    
    if (iterations >= MAX_ITERATIONS) {
        fprintf(stderr, "Error: Maximum iterations reached\n");
        return EXIT_FAILURE;
    }
    
    printf("Approximate square root of 2: %.10f\n", current);
    printf("Error: %.10f\n", ABS(SQUARE(current) - 2.0));
    printf("Iterations: %d\n", iterations);
    
    return EXIT_SUCCESS;
}