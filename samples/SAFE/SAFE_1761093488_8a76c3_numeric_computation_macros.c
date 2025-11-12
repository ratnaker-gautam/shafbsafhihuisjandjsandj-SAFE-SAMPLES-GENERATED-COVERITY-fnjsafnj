//DeepSeek-V3 SAFE v1.4 Category: Safe ; Style: macros ; Variation: numeric_computation
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
    return x * x * x - 2 * x * x + x - 7;
}

double compute_derivative(double x) {
    return 3 * x * x - 4 * x + 1;
}

int main(void) {
    double initial_guess;
    char input_buffer[256];
    
    printf("Enter initial guess for root finding: ");
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
    
    if (!VALIDATE_INPUT(initial_guess)) {
        fprintf(stderr, "Input out of valid range [%g, %g]\n", MIN_INPUT, MAX_INPUT);
        return EXIT_FAILURE;
    }
    
    double x = initial_guess;
    double fx, dfx, delta;
    int iterations = 0;
    
    printf("Iteration %2d: x = %.10f\n", iterations, x);
    
    do {
        fx = compute_function(x);
        dfx = compute_derivative(x);
        
        if (ABS(dfx) < EPSILON) {
            fprintf(stderr, "Derivative too small, cannot continue\n");
            return EXIT_FAILURE;
        }
        
        delta = fx / dfx;
        
        if (!VALIDATE_INPUT(x - delta)) {
            fprintf(stderr, "Computation would exceed valid range\n");
            return EXIT_FAILURE;
        }
        
        x = x - delta;
        iterations++;
        
        printf("Iteration %2d: x = %.10f\n", iterations, x);
        
        if (iterations >= MAX_ITERATIONS) {
            fprintf(stderr, "Maximum iterations reached\n");
            return EXIT_FAILURE;
        }
        
    } while (ABS(delta) > EPSILON && ABS(fx) > EPSILON);
    
    printf("Root found at x = %.10f after %d iterations\n", x, iterations);
    printf("Function value at root: %.10f\n", compute_function(x));
    
    return EXIT_SUCCESS;
}