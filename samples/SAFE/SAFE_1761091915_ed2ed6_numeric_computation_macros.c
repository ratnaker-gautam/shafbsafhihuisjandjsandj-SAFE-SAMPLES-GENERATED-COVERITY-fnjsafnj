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
    return x * x - 2.0;
}

double compute_derivative(double x) {
    return 2.0 * x;
}

int main(void) {
    double initial_guess;
    char input_buffer[256];
    
    printf("Enter initial guess for square root of 2: ");
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
    
    if (ABS(initial_guess) < EPSILON) {
        fprintf(stderr, "Initial guess too close to zero\n");
        return EXIT_FAILURE;
    }
    
    double current = initial_guess;
    double previous;
    int iterations = 0;
    
    printf("Iteration %2d: %.10f\n", iterations, current);
    
    do {
        previous = current;
        double f_val = compute_function(previous);
        double f_deriv = compute_derivative(previous);
        
        if (ABS(f_deriv) < EPSILON) {
            fprintf(stderr, "Derivative too small, method fails\n");
            return EXIT_FAILURE;
        }
        
        current = previous - f_val / f_deriv;
        
        if (!VALIDATE_INPUT(current)) {
            fprintf(stderr, "Computation diverged\n");
            return EXIT_FAILURE;
        }
        
        iterations++;
        printf("Iteration %2d: %.10f\n", iterations, current);
        
        if (iterations >= MAX_ITERATIONS) {
            fprintf(stderr, "Maximum iterations reached\n");
            break;
        }
        
    } while (ABS(current - previous) > EPSILON && ABS(compute_function(current)) > EPSILON);
    
    printf("Final result: %.10f\n", current);
    printf("Error: %.10f\n", ABS(current * current - 2.0));
    
    return EXIT_SUCCESS;
}