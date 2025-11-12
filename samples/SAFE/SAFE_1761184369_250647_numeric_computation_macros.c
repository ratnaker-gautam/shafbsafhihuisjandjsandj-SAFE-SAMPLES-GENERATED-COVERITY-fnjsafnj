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
    double current_x, next_x;
    double function_value, derivative_value;
    int iterations = 0;
    
    printf("Enter initial guess for root finding: ");
    if (scanf("%lf", &initial_guess) != 1) {
        fprintf(stderr, "Error: Invalid input format\n");
        return EXIT_FAILURE;
    }
    
    if (!VALIDATE_INPUT(initial_guess)) {
        fprintf(stderr, "Error: Input out of valid range [%g, %g]\n", MIN_INPUT, MAX_INPUT);
        return EXIT_FAILURE;
    }
    
    current_x = initial_guess;
    
    while (iterations < MAX_ITERATIONS) {
        function_value = compute_function(current_x);
        derivative_value = compute_derivative(current_x);
        
        if (ABS(derivative_value) < EPSILON) {
            fprintf(stderr, "Error: Derivative too small, method fails\n");
            return EXIT_FAILURE;
        }
        
        next_x = current_x - function_value / derivative_value;
        
        if (!VALIDATE_INPUT(next_x)) {
            fprintf(stderr, "Error: Computation resulted in out-of-range value\n");
            return EXIT_FAILURE;
        }
        
        if (ABS(next_x - current_x) < EPSILON) {
            printf("Root found: %.10f\n", next_x);
            printf("Function value at root: %.10f\n", compute_function(next_x));
            printf("Iterations required: %d\n", iterations + 1);
            return EXIT_SUCCESS;
        }
        
        current_x = next_x;
        iterations++;
    }
    
    fprintf(stderr, "Error: Maximum iterations reached without convergence\n");
    return EXIT_FAILURE;
}