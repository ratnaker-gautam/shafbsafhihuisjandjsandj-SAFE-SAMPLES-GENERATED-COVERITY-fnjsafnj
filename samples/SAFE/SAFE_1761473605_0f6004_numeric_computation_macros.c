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
    return x * x - 2.0;
}

double compute_derivative(double x) {
    return 2.0 * x;
}

int main(void) {
    double initial_guess;
    char input_buffer[256];
    
    printf("Enter initial guess for Newton's method (between %.0f and %.0f): ", MIN_INPUT, MAX_INPUT);
    
    if (fgets(input_buffer, sizeof(input_buffer), stdin) == NULL) {
        fprintf(stderr, "Error reading input\n");
        return EXIT_FAILURE;
    }
    
    char *endptr;
    initial_guess = strtod(input_buffer, &endptr);
    if (endptr == input_buffer || *endptr != '\n') {
        fprintf(stderr, "Invalid input format\n");
        return EXIT_FAILURE;
    }
    
    if (!IS_VALID(initial_guess)) {
        fprintf(stderr, "Input out of valid range\n");
        return EXIT_FAILURE;
    }
    
    if (fabs(initial_guess) < EPSILON) {
        fprintf(stderr, "Initial guess too close to zero\n");
        return EXIT_FAILURE;
    }
    
    double x = initial_guess;
    double prev_x;
    int iterations = 0;
    
    printf("Iteration %d: x = %.10f, f(x) = %.10f\n", iterations, x, compute_function(x));
    
    while (iterations < MAX_ITERATIONS) {
        double fx = compute_function(x);
        double dfx = compute_derivative(x);
        
        if (fabs(dfx) < EPSILON) {
            fprintf(stderr, "Derivative too small, method fails\n");
            return EXIT_FAILURE;
        }
        
        prev_x = x;
        double delta = fx / dfx;
        
        if (fabs(delta) > fabs(prev_x) * 1e3) {
            fprintf(stderr, "Step size too large, method unstable\n");
            return EXIT_FAILURE;
        }
        
        x = prev_x - delta;
        
        iterations++;
        printf("Iteration %d: x = %.10f, f(x) = %.10f\n", iterations, x, compute_function(x));
        
        if (fabs(x - prev_x) < EPSILON) {
            printf("Converged after %d iterations\n", iterations);
            printf("Root approximation: %.10f\n", x);
            printf("Function value at root: %.10f\n", compute_function(x));
            return EXIT_SUCCESS;
        }
        
        if (!IS_VALID(x)) {
            fprintf(stderr, "Computation diverged\n");
            return EXIT_FAILURE;
        }
    }
    
    fprintf(stderr, "Failed to converge within %d iterations\n", MAX_ITERATIONS);
    return EXIT_FAILURE;
}