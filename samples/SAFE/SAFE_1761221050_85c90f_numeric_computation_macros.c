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
    double x0, x1, fx0, fpx0;
    int iterations = 0;
    int converged = 0;
    
    printf("Enter initial guess for Newton-Raphson method: ");
    if (scanf("%lf", &initial_guess) != 1) {
        fprintf(stderr, "Error: Invalid input format\n");
        return EXIT_FAILURE;
    }
    
    if (!validate_input(initial_guess)) {
        fprintf(stderr, "Error: Input value out of valid range [%g, %g]\n", MIN_INPUT, MAX_INPUT);
        return EXIT_FAILURE;
    }
    
    x0 = initial_guess;
    
    while (iterations < MAX_ITERATIONS) {
        fx0 = compute_function(x0);
        fpx0 = compute_derivative(x0);
        
        if (fabs(fpx0) < EPSILON) {
            fprintf(stderr, "Error: Derivative too close to zero\n");
            return EXIT_FAILURE;
        }
        
        x1 = x0 - fx0 / fpx0;
        
        if (!validate_input(x1)) {
            fprintf(stderr, "Error: Computation resulted in invalid value\n");
            return EXIT_FAILURE;
        }
        
        if (fabs(x1 - x0) < EPSILON) {
            converged = 1;
            break;
        }
        
        x0 = x1;
        iterations++;
    }
    
    if (converged) {
        printf("Root found: %.10f\n", x1);
        printf("Function value at root: %.10f\n", compute_function(x1));
        printf("Iterations required: %d\n", iterations);
    } else {
        fprintf(stderr, "Error: Failed to converge within %d iterations\n", MAX_ITERATIONS);
        return EXIT_FAILURE;
    }
    
    return EXIT_SUCCESS;
}