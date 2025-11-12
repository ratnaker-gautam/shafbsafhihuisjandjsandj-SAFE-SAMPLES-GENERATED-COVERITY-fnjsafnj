//DeepSeek-V3 V2.5 Category: Safe ; Style: monolithic_main ; Variation: numeric_computation
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#define MAX_ITERATIONS 1000
#define TOLERANCE 1e-10

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
        return 1;
    }
    
    char *endptr;
    initial_guess = strtod(buffer, &endptr);
    if (endptr == buffer || *endptr != '\n') {
        fprintf(stderr, "Invalid input: must be a number\n");
        return 1;
    }
    
    if (!isfinite(initial_guess)) {
        fprintf(stderr, "Invalid input: number must be finite\n");
        return 1;
    }
    
    double x = initial_guess;
    double fx = compute_function(x);
    double fpx = compute_derivative(x);
    
    if (fabs(fpx) < TOLERANCE) {
        fprintf(stderr, "Derivative too small at initial guess\n");
        return 1;
    }
    
    int iterations = 0;
    while (iterations < MAX_ITERATIONS) {
        if (fabs(fpx) < TOLERANCE) {
            fprintf(stderr, "Derivative became too small\n");
            return 1;
        }
        
        double delta = fx / fpx;
        if (!isfinite(delta)) {
            fprintf(stderr, "Numerical instability detected\n");
            return 1;
        }
        
        x = x - delta;
        
        if (fabs(delta) < TOLERANCE) {
            break;
        }
        
        fx = compute_function(x);
        fpx = compute_derivative(x);
        iterations++;
    }
    
    if (iterations >= MAX_ITERATIONS) {
        fprintf(stderr, "Maximum iterations reached without convergence\n");
        return 1;
    }
    
    double final_value = compute_function(x);
    if (fabs(final_value) > 0.1) {
        fprintf(stderr, "Solution may not be accurate\n");
        return 1;
    }
    
    printf("Found root at x = %.10f\n", x);
    printf("Function value at root: %.10f\n", final_value);
    printf("Iterations required: %d\n", iterations);
    
    return 0;
}