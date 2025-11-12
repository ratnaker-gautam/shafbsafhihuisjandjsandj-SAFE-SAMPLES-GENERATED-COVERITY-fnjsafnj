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

int find_root_newton(double initial_guess, double *result) {
    if (!VALIDATE_INPUT(initial_guess)) {
        return 0;
    }
    
    double x = initial_guess;
    int iterations = 0;
    
    while (iterations < MAX_ITERATIONS) {
        double fx = compute_function(x);
        double dfx = compute_derivative(x);
        
        if (ABS(dfx) < EPSILON) {
            return 0;
        }
        
        double delta = fx / dfx;
        if (ABS(delta) < EPSILON) {
            *result = x;
            return 1;
        }
        
        double new_x = x - delta;
        if (!VALIDATE_INPUT(new_x)) {
            return 0;
        }
        
        x = new_x;
        iterations++;
    }
    
    return 0;
}

int main(void) {
    double initial_guess;
    
    printf("Enter initial guess for root finding (-1000000 to 1000000): ");
    if (scanf("%lf", &initial_guess) != 1) {
        printf("Invalid input format.\n");
        return EXIT_FAILURE;
    }
    
    if (!VALIDATE_INPUT(initial_guess)) {
        printf("Input out of valid range.\n");
        return EXIT_FAILURE;
    }
    
    double root;
    if (find_root_newton(initial_guess, &root)) {
        printf("Found root: %.10f\n", root);
        printf("Function value at root: %.10f\n", compute_function(root));
    } else {
        printf("Failed to find root with given initial guess.\n");
        return EXIT_FAILURE;
    }
    
    return EXIT_SUCCESS;
}