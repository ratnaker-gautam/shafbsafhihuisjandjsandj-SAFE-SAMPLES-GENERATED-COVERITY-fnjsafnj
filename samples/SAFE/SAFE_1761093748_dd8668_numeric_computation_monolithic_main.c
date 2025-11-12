//DeepSeek-V3 SAFE v1.4 Category: Safe ; Style: monolithic_main ; Variation: numeric_computation
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#define MAX_ITERATIONS 1000
#define TOLERANCE 1e-10

double compute_function(double x) {
    return x * x * x - 2 * x * x - 5 * x + 6;
}

double compute_derivative(double x) {
    return 3 * x * x - 4 * x - 5;
}

int main(void) {
    double initial_guess;
    double x, x_new, fx, dfx;
    int iterations = 0;
    
    printf("Enter initial guess for root finding: ");
    if (scanf("%lf", &initial_guess) != 1) {
        fprintf(stderr, "Error: Invalid input\n");
        return EXIT_FAILURE;
    }
    
    if (!isfinite(initial_guess)) {
        fprintf(stderr, "Error: Invalid initial guess\n");
        return EXIT_FAILURE;
    }
    
    x = initial_guess;
    
    while (iterations < MAX_ITERATIONS) {
        fx = compute_function(x);
        dfx = compute_derivative(x);
        
        if (fabs(dfx) < TOLERANCE) {
            fprintf(stderr, "Error: Derivative too small\n");
            return EXIT_FAILURE;
        }
        
        if (!isfinite(fx) || !isfinite(dfx)) {
            fprintf(stderr, "Error: Numerical overflow\n");
            return EXIT_FAILURE;
        }
        
        x_new = x - fx / dfx;
        
        if (!isfinite(x_new)) {
            fprintf(stderr, "Error: Numerical overflow in iteration\n");
            return EXIT_FAILURE;
        }
        
        if (fabs(x_new - x) < TOLERANCE) {
            printf("Root found: %.10f\n", x_new);
            printf("Function value at root: %.10f\n", compute_function(x_new));
            printf("Iterations required: %d\n", iterations + 1);
            return EXIT_SUCCESS;
        }
        
        x = x_new;
        iterations++;
    }
    
    fprintf(stderr, "Error: Maximum iterations reached\n");
    return EXIT_FAILURE;
}