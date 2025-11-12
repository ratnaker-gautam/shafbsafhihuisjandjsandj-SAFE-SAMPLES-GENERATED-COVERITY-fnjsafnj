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
    double x0, x1, f0, f1;
    int iterations = 0;
    
    printf("Enter initial guess for root finding: ");
    if (scanf("%lf", &initial_guess) != 1) {
        fprintf(stderr, "Error: Invalid input format\n");
        return EXIT_FAILURE;
    }
    
    if (!IS_VALID(initial_guess)) {
        fprintf(stderr, "Error: Input out of valid range [%g, %g]\n", MIN_INPUT, MAX_INPUT);
        return EXIT_FAILURE;
    }
    
    x0 = initial_guess;
    
    while (iterations < MAX_ITERATIONS) {
        f0 = compute_function(x0);
        f1 = compute_derivative(x0);
        
        if (fabs(f1) < EPSILON) {
            fprintf(stderr, "Error: Derivative too small, method fails\n");
            return EXIT_FAILURE;
        }
        
        x1 = x0 - f0 / f1;
        
        if (!IS_VALID(x1)) {
            fprintf(stderr, "Error: Computation resulted in out-of-range value\n");
            return EXIT_FAILURE;
        }
        
        if (fabs(x1 - x0) < EPSILON) {
            printf("Root found: %.10f\n", x1);
            printf("Function value at root: %.10f\n", compute_function(x1));
            printf("Iterations required: %d\n", iterations + 1);
            return EXIT_SUCCESS;
        }
        
        x0 = x1;
        iterations++;
    }
    
    fprintf(stderr, "Error: Maximum iterations reached without convergence\n");
    return EXIT_FAILURE;
}