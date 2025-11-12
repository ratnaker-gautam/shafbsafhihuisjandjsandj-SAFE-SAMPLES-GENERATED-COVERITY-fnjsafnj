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
    return x * x * x - 2 * x * x + x - 7;
}

double compute_derivative(double x) {
    return 3 * x * x - 4 * x + 1;
}

int main(void) {
    double initial_guess;
    double x, x_prev, f_x, f_prime_x;
    int iterations = 0;
    
    printf("Enter initial guess for root finding: ");
    if (scanf("%lf", &initial_guess) != 1) {
        fprintf(stderr, "Error: Invalid input format\n");
        return EXIT_FAILURE;
    }
    
    if (!IS_VALID(initial_guess)) {
        fprintf(stderr, "Error: Input out of valid range [%g, %g]\n", MIN_INPUT, MAX_INPUT);
        return EXIT_FAILURE;
    }
    
    x = initial_guess;
    
    do {
        f_x = compute_function(x);
        f_prime_x = compute_derivative(x);
        
        if (fabs(f_prime_x) < EPSILON) {
            fprintf(stderr, "Error: Derivative too small, method fails\n");
            return EXIT_FAILURE;
        }
        
        x_prev = x;
        x = x_prev - f_x / f_prime_x;
        
        iterations++;
        
        if (iterations > MAX_ITERATIONS) {
            fprintf(stderr, "Error: Maximum iterations reached\n");
            return EXIT_FAILURE;
        }
        
        if (!IS_VALID(x)) {
            fprintf(stderr, "Error: Computation diverged\n");
            return EXIT_FAILURE;
        }
        
    } while (fabs(x - x_prev) > EPSILON && fabs(f_x) > EPSILON);
    
    printf("Approximate root: %.10f\n", x);
    printf("Function value at root: %.10f\n", compute_function(x));
    printf("Iterations required: %d\n", iterations);
    
    return EXIT_SUCCESS;
}