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
#define SQUARE(x) ((x) * (x))
#define CUBE(x) ((x) * (x) * (x))

double compute_polynomial(double x) {
    return 2.0 * SQUARE(x) - 3.0 * x + 1.5;
}

double compute_derivative(double x) {
    return 4.0 * x - 3.0;
}

int find_root_newton(double initial_guess, double* result) {
    if (!VALIDATE_INPUT(initial_guess)) {
        return 0;
    }
    
    double x = initial_guess;
    int iterations = 0;
    
    while (iterations < MAX_ITERATIONS) {
        double fx = compute_polynomial(x);
        double dfx = compute_derivative(x);
        
        if (fabs(dfx) < EPSILON) {
            return 0;
        }
        
        double delta = fx / dfx;
        if (fabs(delta) < EPSILON) {
            *result = x;
            return 1;
        }
        
        x = x - delta;
        
        if (!VALIDATE_INPUT(x)) {
            return 0;
        }
        
        iterations++;
    }
    
    return 0;
}

int main() {
    double initial_guess;
    printf("Enter initial guess for root finding (-1000000 to 1000000): ");
    
    if (scanf("%lf", &initial_guess) != 1) {
        printf("Error: Invalid input format\n");
        return 1;
    }
    
    if (!VALIDATE_INPUT(initial_guess)) {
        printf("Error: Input out of valid range\n");
        return 1;
    }
    
    double root;
    if (find_root_newton(initial_guess, &root)) {
        printf("Found root at: %.10f\n", root);
        printf("Function value at root: %.10f\n", compute_polynomial(root));
    } else {
        printf("Failed to find root with given initial guess\n");
        return 1;
    }
    
    printf("\nAdditional polynomial evaluations:\n");
    printf("f(0.0) = %.6f\n", compute_polynomial(0.0));
    printf("f(1.0) = %.6f\n", compute_polynomial(1.0));
    printf("f(2.0) = %.6f\n", compute_polynomial(2.0));
    
    return 0;
}