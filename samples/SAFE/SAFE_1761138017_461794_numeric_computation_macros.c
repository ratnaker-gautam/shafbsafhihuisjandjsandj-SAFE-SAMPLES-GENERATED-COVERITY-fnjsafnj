//DeepSeek-V3 V2.5 Category: Safe ; Style: macros ; Variation: numeric_computation
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
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

int find_root_newton(double initial_guess, double *result) {
    double x = initial_guess;
    double fx, dfx;
    int iterations = 0;
    
    while (iterations < MAX_ITERATIONS) {
        fx = compute_function(x);
        if (ABS(fx) < EPSILON) {
            *result = x;
            return 1;
        }
        
        dfx = compute_derivative(x);
        if (ABS(dfx) < EPSILON) {
            return 0;
        }
        
        double delta = fx / dfx;
        if (ABS(delta) > 1e10) {
            return 0;
        }
        
        x = x - delta;
        iterations++;
    }
    
    return 0;
}

int main(void) {
    double initial_guess;
    char input_buffer[100];
    char *endptr;
    
    printf("Enter initial guess for root finding: ");
    if (fgets(input_buffer, sizeof(input_buffer), stdin) == NULL) {
        fprintf(stderr, "Error reading input\n");
        return EXIT_FAILURE;
    }
    
    initial_guess = strtod(input_buffer, &endptr);
    if (endptr == input_buffer) {
        fprintf(stderr, "Invalid input: not a number\n");
        return EXIT_FAILURE;
    }
    
    if (!VALIDATE_INPUT(initial_guess)) {
        fprintf(stderr, "Input out of valid range [%g, %g]\n", MIN_INPUT, MAX_INPUT);
        return EXIT_FAILURE;
    }
    
    double root;
    if (find_root_newton(initial_guess, &root)) {
        printf("Found root: %.10g\n", root);
        printf("Function value at root: %.10g\n", compute_function(root));
    } else {
        printf("Failed to find root with given initial guess\n");
        return EXIT_FAILURE;
    }
    
    return EXIT_SUCCESS;
}