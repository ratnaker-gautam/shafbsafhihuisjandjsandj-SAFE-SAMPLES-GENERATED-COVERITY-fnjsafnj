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
    
    if (!VALIDATE_INPUT(initial_guess)) {
        fprintf(stderr, "Input out of valid range [%g, %g]\n", MIN_INPUT, MAX_INPUT);
        return EXIT_FAILURE;
    }
    
    double x = initial_guess;
    double fx, dfx, delta;
    int iterations = 0;
    
    while (iterations < MAX_ITERATIONS) {
        fx = compute_function(x);
        if (ABS(fx) < EPSILON) {
            break;
        }
        
        dfx = compute_derivative(x);
        if (ABS(dfx) < EPSILON) {
            fprintf(stderr, "Derivative too small, method fails\n");
            return EXIT_FAILURE;
        }
        
        delta = fx / dfx;
        if (!VALIDATE_INPUT(x - delta)) {
            fprintf(stderr, "Numerical overflow detected\n");
            return EXIT_FAILURE;
        }
        
        x = x - delta;
        iterations++;
        
        if (ABS(delta) < EPSILON) {
            break;
        }
    }
    
    if (iterations >= MAX_ITERATIONS) {
        fprintf(stderr, "Maximum iterations reached\n");
        return EXIT_FAILURE;
    }
    
    printf("Approximate root: %.10g\n", x);
    printf("Function value at root: %.10g\n", compute_function(x));
    printf("Iterations: %d\n", iterations);
    
    return EXIT_SUCCESS;
}