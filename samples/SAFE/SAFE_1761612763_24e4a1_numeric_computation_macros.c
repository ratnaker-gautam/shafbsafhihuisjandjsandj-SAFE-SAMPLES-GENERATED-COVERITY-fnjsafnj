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
    double fx = compute_function(x);
    
    if (ABS(fx) < EPSILON) {
        printf("Root found at: %.10g\n", x);
        return EXIT_SUCCESS;
    }
    
    int iterations = 0;
    while (iterations < MAX_ITERATIONS) {
        double dfx = compute_derivative(x);
        
        if (ABS(dfx) < EPSILON) {
            fprintf(stderr, "Derivative too small, method fails\n");
            return EXIT_FAILURE;
        }
        
        double x_new = x - fx / dfx;
        
        if (!VALIDATE_INPUT(x_new)) {
            fprintf(stderr, "Computation resulted in out-of-range value\n");
            return EXIT_FAILURE;
        }
        
        double fx_new = compute_function(x_new);
        
        if (ABS(fx_new) < EPSILON) {
            printf("Root found at: %.10g\n", x_new);
            return EXIT_SUCCESS;
        }
        
        if (ABS(x_new - x) < EPSILON) {
            printf("Converged to root: %.10g\n", x_new);
            return EXIT_SUCCESS;
        }
        
        x = x_new;
        fx = fx_new;
        iterations++;
    }
    
    fprintf(stderr, "Maximum iterations reached without convergence\n");
    return EXIT_FAILURE;
}