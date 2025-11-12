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

int validate_input(double input) {
    if (!IS_VALID(input)) {
        return 0;
    }
    return 1;
}

int main(void) {
    double initial_guess;
    double current_value;
    double next_value;
    double function_value;
    double derivative_value;
    int iterations;
    int valid_input;
    
    printf("Enter initial guess for root finding: ");
    valid_input = scanf("%lf", &initial_guess);
    
    if (valid_input != 1) {
        fprintf(stderr, "Error: Invalid input format\n");
        return EXIT_FAILURE;
    }
    
    if (!validate_input(initial_guess)) {
        fprintf(stderr, "Error: Input out of valid range [%f, %f]\n", MIN_INPUT, MAX_INPUT);
        return EXIT_FAILURE;
    }
    
    current_value = initial_guess;
    iterations = 0;
    
    while (iterations < MAX_ITERATIONS) {
        function_value = compute_function(current_value);
        derivative_value = compute_derivative(current_value);
        
        if (fabs(derivative_value) < EPSILON) {
            fprintf(stderr, "Error: Derivative too close to zero\n");
            return EXIT_FAILURE;
        }
        
        next_value = current_value - function_value / derivative_value;
        
        if (!validate_input(next_value)) {
            fprintf(stderr, "Error: Computation resulted in out-of-range value\n");
            return EXIT_FAILURE;
        }
        
        if (fabs(next_value - current_value) < EPSILON) {
            break;
        }
        
        current_value = next_value;
        iterations++;
    }
    
    if (iterations >= MAX_ITERATIONS) {
        fprintf(stderr, "Error: Maximum iterations reached without convergence\n");
        return EXIT_FAILURE;
    }
    
    function_value = compute_function(current_value);
    
    if (fabs(function_value) > 1e-5) {
        fprintf(stderr, "Warning: Solution may not be accurate\n");
    }
    
    printf("Approximate root: %.10f\n", current_value);
    printf("Function value at root: %.10f\n", function_value);
    printf("Iterations required: %d\n", iterations);
    
    return EXIT_SUCCESS;
}