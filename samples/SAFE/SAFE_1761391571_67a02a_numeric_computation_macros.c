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
#define SQUARE(x) ((x) * (x))
#define CUBE(x) ((x) * (x) * (x))

double compute_polynomial(double x) {
    return 2.0 * CUBE(x) - 3.0 * SQUARE(x) + 4.0 * x - 5.0;
}

double compute_derivative(double x) {
    return 6.0 * SQUARE(x) - 6.0 * x + 4.0;
}

int find_root_newton(double guess, double *result) {
    double x = guess;
    int iterations = 0;
    
    while (iterations < MAX_ITERATIONS) {
        double fx = compute_polynomial(x);
        double dfx = compute_derivative(x);
        
        if (fabs(dfx) < EPSILON) {
            return -1;
        }
        
        double delta = fx / dfx;
        if (fabs(delta) < EPSILON) {
            *result = x;
            return 0;
        }
        
        x = x - delta;
        iterations++;
    }
    
    return -2;
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
    
    if (!VALIDATE_INPUT(initial_guess)) {
        fprintf(stderr, "Input out of valid range\n");
        return EXIT_FAILURE;
    }
    
    if (isnan(initial_guess) || isinf(initial_guess)) {
        fprintf(stderr, "Input is not a finite number\n");
        return EXIT_FAILURE;
    }
    
    double root;
    int result = find_root_newton(initial_guess, &root);
    
    if (result == 0) {
        printf("Found root at: %.10f\n", root);
        printf("Function value at root: %.10f\n", compute_polynomial(root));
    } else if (result == -1) {
        fprintf(stderr, "Derivative too small, cannot continue\n");
        return EXIT_FAILURE;
    } else {
        fprintf(stderr, "Maximum iterations reached, no convergence\n");
        return EXIT_FAILURE;
    }
    
    return EXIT_SUCCESS;
}