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
    char buffer[256];
    
    printf("Enter initial guess for Newton's method (between %.1f and %.1f): ", MIN_INPUT, MAX_INPUT);
    
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
    
    while (iterations < MAX_ITERATIONS) {
        double fx = compute_function(x);
        double dfx = compute_derivative(x);
        
        if (fabs(dfx) < EPSILON) {
            fprintf(stderr, "Derivative too small\n");
            return EXIT_FAILURE;
        }
        
        prev_x = x;
        x = x - fx / dfx;
        
        if (!IS_VALID(x)) {
            fprintf(stderr, "Computation resulted in out-of-range value\n");
            return EXIT_FAILURE;
        }
        
        if (fabs(x - prev_x) < EPSILON) {
            break;
        }
        
        iterations++;
    }
    
    if (iterations >= MAX_ITERATIONS) {
        fprintf(stderr, "Maximum iterations reached\n");
        return EXIT_FAILURE;
    }
    
    printf("Approximate root: %.10f\n", x);
    printf("Function value at root: %.10f\n", compute_function(x));
    printf("Iterations: %d\n", iterations);
    
    return EXIT_SUCCESS;
}