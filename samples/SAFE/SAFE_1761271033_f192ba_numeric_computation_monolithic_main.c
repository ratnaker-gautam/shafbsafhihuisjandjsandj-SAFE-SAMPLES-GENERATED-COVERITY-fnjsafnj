//DeepSeek-V3 V2.5 Category: Safe ; Style: monolithic_main ; Variation: numeric_computation
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#define MAX_ITERATIONS 1000
#define TOLERANCE 1e-10

double compute_function(double x) {
    return x*x*x - 2*x*x - 5*x + 6;
}

double compute_derivative(double x) {
    return 3*x*x - 4*x - 5;
}

int main(void) {
    double initial_guess;
    char input_buffer[256];
    
    printf("Enter initial guess for root finding: ");
    if (fgets(input_buffer, sizeof(input_buffer), stdin) == NULL) {
        fprintf(stderr, "Error reading input\n");
        return 1;
    }
    
    char *endptr;
    initial_guess = strtod(input_buffer, &endptr);
    if (endptr == input_buffer || *endptr != '\n') {
        fprintf(stderr, "Invalid input: must be a number\n");
        return 1;
    }
    
    if (!isfinite(initial_guess)) {
        fprintf(stderr, "Invalid input: number must be finite\n");
        return 1;
    }
    
    double x = initial_guess;
    double fx, dfx, delta;
    int iterations = 0;
    
    printf("Iteration %2d: x = %.10f\n", iterations, x);
    
    while (iterations < MAX_ITERATIONS) {
        fx = compute_function(x);
        dfx = compute_derivative(x);
        
        if (fabs(dfx) < TOLERANCE) {
            fprintf(stderr, "Derivative too small, cannot continue\n");
            return 1;
        }
        
        delta = fx / dfx;
        
        if (fabs(delta) < TOLERANCE) {
            break;
        }
        
        x = x - delta;
        
        if (!isfinite(x)) {
            fprintf(stderr, "Numerical instability detected\n");
            return 1;
        }
        
        iterations++;
        printf("Iteration %2d: x = %.10f, f(x) = %.10f\n", iterations, x, compute_function(x));
    }
    
    if (iterations >= MAX_ITERATIONS) {
        fprintf(stderr, "Maximum iterations reached without convergence\n");
        return 1;
    }
    
    printf("\nRoot found: x = %.10f\n", x);
    printf("Function value at root: f(x) = %.10f\n", compute_function(x));
    printf("Iterations required: %d\n", iterations);
    
    return 0;
}