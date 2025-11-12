//DeepSeek-V3 SAFE v1.3 Category: Safe ; Style: macros ; Variation: numeric_computation
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
    return x * x - 2.0;
}

double compute_derivative(double x) {
    return 2.0 * x;
}

int main(void) {
    double initial_guess;
    double current;
    double next;
    int iterations;
    
    printf("Enter initial guess for square root of 2: ");
    if (scanf("%lf", &initial_guess) != 1) {
        fprintf(stderr, "Error: Invalid input format\n");
        return EXIT_FAILURE;
    }
    
    if (!IS_VALID(initial_guess)) {
        fprintf(stderr, "Error: Input out of valid range [%g, %g]\n", MIN_INPUT, MAX_INPUT);
        return EXIT_FAILURE;
    }
    
    if (fabs(initial_guess) < EPSILON) {
        fprintf(stderr, "Error: Initial guess too close to zero\n");
        return EXIT_FAILURE;
    }
    
    current = initial_guess;
    iterations = 0;
    
    while (iterations < MAX_ITERATIONS) {
        double fx = compute_function(current);
        double dfx = compute_derivative(current);
        
        if (fabs(dfx) < EPSILON) {
            fprintf(stderr, "Error: Derivative too small\n");
            return EXIT_FAILURE;
        }
        
        next = current - fx / dfx;
        
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
        fprintf(stderr, "Error: Maximum iterations reached\n");
        return EXIT_FAILURE;
    }
    
    printf("Approximate square root of 2: %.10f\n", current);
    printf("Iterations required: %d\n", iterations);
    printf("Error: %.10f\n", fabs(current * current - 2.0));
    
    return EXIT_SUCCESS;
}