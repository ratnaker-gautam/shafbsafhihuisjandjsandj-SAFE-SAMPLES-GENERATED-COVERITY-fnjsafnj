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
    char input_buffer[256];
    
    printf("Enter initial guess for root finding (-1000000 to 1000000): ");
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
    
    if (fabs(compute_derivative(initial_guess)) < EPSILON) {
        fprintf(stderr, "Derivative too small at initial guess\n");
        return EXIT_FAILURE;
    }
    
    double x = initial_guess;
    double x_prev;
    int iterations = 0;
    
    printf("Iteration %d: x = %.10f, f(x) = %.10f\n", iterations, x, compute_function(x));
    
    do {
        x_prev = x;
        double fx = compute_function(x_prev);
        double dfx = compute_derivative(x_prev);
        
        if (fabs(dfx) < EPSILON) {
            fprintf(stderr, "Derivative became too small\n");
            return EXIT_FAILURE;
        }
        
        double delta = fx / dfx;
        if (fabs(delta) > 1e100) {
            fprintf(stderr, "Divergence detected\n");
            return EXIT_FAILURE;
        }
        
        x = x_prev - delta;
        
        iterations++;
        printf("Iteration %d: x = %.10f, f(x) = %.10f\n", iterations, x, compute_function(x));
        
        if (iterations >= MAX_ITERATIONS) {
            fprintf(stderr, "Maximum iterations reached\n");
            return EXIT_FAILURE;
        }
        
    } while (fabs(x - x_prev) > EPSILON && fabs(compute_function(x)) > EPSILON);
    
    printf("Root found: x = %.10f\n", x);
    printf("Final function value: f(x) = %.10f\n", compute_function(x));
    printf("Iterations required: %d\n", iterations);
    
    return EXIT_SUCCESS;
}