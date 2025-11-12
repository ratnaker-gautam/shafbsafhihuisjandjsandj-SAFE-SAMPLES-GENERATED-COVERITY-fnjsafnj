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

int main(void) {
    double initial_guess;
    double current;
    double next;
    int iterations;
    
    printf("Enter initial guess for root finding: ");
    if (scanf("%lf", &initial_guess) != 1) {
        fprintf(stderr, "Error: Invalid input format\n");
        return EXIT_FAILURE;
    }
    
    if (!IS_VALID(initial_guess)) {
        fprintf(stderr, "Error: Input out of valid range [%g, %g]\n", MIN_INPUT, MAX_INPUT);
        return EXIT_FAILURE;
    }
    
    current = initial_guess;
    iterations = 0;
    
    while (iterations < MAX_ITERATIONS) {
        double derivative = compute_derivative(current);
        double function_value = compute_function(current);
        
        if (fabs(derivative) < EPSILON) {
            fprintf(stderr, "Error: Derivative too small, method fails\n");
            return EXIT_FAILURE;
        }
        
        next = current - function_value / derivative;
        
        if (!IS_VALID(next)) {
            fprintf(stderr, "Error: Computation resulted in out-of-range value\n");
            return EXIT_FAILURE;
        }
        
        if (fabs(next - current) < EPSILON) {
            break;
        }
        
        current = next;
        iterations++;
    }
    
    if (iterations >= MAX_ITERATIONS) {
        fprintf(stderr, "Error: Maximum iterations reached without convergence\n");
        return EXIT_FAILURE;
    }
    
    printf("Approximate root: %.10f\n", current);
    printf("Function value at root: %.10f\n", compute_function(current));
    printf("Iterations required: %d\n", iterations);
    
    return EXIT_SUCCESS;
}