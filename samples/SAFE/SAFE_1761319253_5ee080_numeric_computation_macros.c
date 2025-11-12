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
    char buffer[256];
    
    printf("Enter initial guess for root finding: ");
    if (fgets(buffer, sizeof(buffer), stdin) == NULL) {
        fprintf(stderr, "Error reading input\n");
        return EXIT_FAILURE;
    }
    
    char *endptr;
    initial_guess = strtod(buffer, &endptr);
    if (endptr == buffer || *endptr != '\n') {
        fprintf(stderr, "Invalid input format\n");
        return EXIT_FAILURE;
    }
    
    if (!IS_VALID(initial_guess)) {
        fprintf(stderr, "Input out of valid range [%g, %g]\n", MIN_INPUT, MAX_INPUT);
        return EXIT_FAILURE;
    }
    
    double x = initial_guess;
    double fx = compute_function(x);
    double dfx = compute_derivative(x);
    
    if (fabs(dfx) < EPSILON) {
        fprintf(stderr, "Derivative too small, cannot proceed\n");
        return EXIT_FAILURE;
    }
    
    int iterations = 0;
    while (fabs(fx) > EPSILON && iterations < MAX_ITERATIONS) {
        double delta = fx / dfx;
        if (!isfinite(delta) || fabs(delta) > 1e10) {
            fprintf(stderr, "Numerical instability detected\n");
            return EXIT_FAILURE;
        }
        
        x = x - delta;
        if (!IS_VALID(x)) {
            fprintf(stderr, "Computation exceeded valid range\n");
            return EXIT_FAILURE;
        }
        
        fx = compute_function(x);
        dfx = compute_derivative(x);
        
        if (fabs(dfx) < EPSILON) {
            fprintf(stderr, "Derivative became too small\n");
            return EXIT_FAILURE;
        }
        
        iterations++;
    }
    
    if (iterations >= MAX_ITERATIONS) {
        fprintf(stderr, "Maximum iterations reached without convergence\n");
        return EXIT_FAILURE;
    }
    
    if (!isfinite(x) || !isfinite(fx)) {
        fprintf(stderr, "Invalid result computed\n");
        return EXIT_FAILURE;
    }
    
    printf("Root found at x = %.10g\n", x);
    printf("Function value at root: %.10g\n", fx);
    printf("Iterations required: %d\n", iterations);
    
    return EXIT_SUCCESS;
}