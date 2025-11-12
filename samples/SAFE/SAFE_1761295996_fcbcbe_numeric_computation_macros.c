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
    
    printf("Enter initial guess for square root of 2: ");
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
        fprintf(stderr, "Input out of valid range [%g, %g]\n", MIN_INPUT, MAX_INPUT);
        return EXIT_FAILURE;
    }
    
    if (fabs(initial_guess) < EPSILON) {
        fprintf(stderr, "Initial guess too close to zero\n");
        return EXIT_FAILURE;
    }
    
    double x = initial_guess;
    int iterations = 0;
    
    while (iterations < MAX_ITERATIONS) {
        double fx = compute_function(x);
        double dfx = compute_derivative(x);
        
        if (fabs(dfx) < EPSILON) {
            fprintf(stderr, "Derivative too small\n");
            return EXIT_FAILURE;
        }
        
        double delta = fx / dfx;
        
        if (!IS_VALID(delta) || !IS_VALID(x - delta)) {
            fprintf(stderr, "Numerical overflow detected\n");
            return EXIT_FAILURE;
        }
        
        double x_new = x - delta;
        
        if (fabs(x_new - x) < EPSILON) {
            printf("Converged after %d iterations\n", iterations);
            printf("Approximate square root of 2: %.10g\n", x_new);
            printf("Verification: %.10g^2 = %.10g\n", x_new, x_new * x_new);
            return EXIT_SUCCESS;
        }
        
        x = x_new;
        iterations++;
    }
    
    fprintf(stderr, "Failed to converge within %d iterations\n", MAX_ITERATIONS);
    return EXIT_FAILURE;
}