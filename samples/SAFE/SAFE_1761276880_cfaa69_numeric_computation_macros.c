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
    int converged = 0;
    
    printf("Enter initial guess for root finding (between %.1f and %.1f): ", MIN_INPUT, MAX_INPUT);
    
    if (scanf("%lf", &initial_guess) != 1) {
        fprintf(stderr, "Error: Invalid input format\n");
        return EXIT_FAILURE;
    }
    
    if (!IS_VALID(initial_guess)) {
        fprintf(stderr, "Error: Input out of valid range\n");
        return EXIT_FAILURE;
    }
    
    x = initial_guess;
    
    while (iterations < MAX_ITERATIONS) {
        f_x = compute_function(x);
        f_prime_x = compute_derivative(x);
        
        if (fabs(f_prime_x) < EPSILON) {
            fprintf(stderr, "Error: Derivative too small, method fails\n");
            return EXIT_FAILURE;
        }
        
        x_prev = x;
        x = x_prev - f_x / f_prime_x;
        
        if (!IS_VALID(x)) {
            fprintf(stderr, "Error: Computation resulted in out-of-range value\n");
            return EXIT_FAILURE;
        }
        
        if (fabs(x - x_prev) < EPSILON) {
            converged = 1;
            break;
        }
        
        iterations++;
    }
    
    if (converged) {
        printf("Root found: %.10f\n", x);
        printf("Function value at root: %.10f\n", compute_function(x));
        printf("Iterations required: %d\n", iterations);
    } else {
        fprintf(stderr, "Error: Method did not converge within %d iterations\n", MAX_ITERATIONS);
        return EXIT_FAILURE;
    }
    
    return EXIT_SUCCESS;
}