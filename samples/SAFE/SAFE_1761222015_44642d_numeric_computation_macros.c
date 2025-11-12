//DeepSeek-V3 V2.5 Category: Safe ; Style: macros ; Variation: numeric_computation
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <limits.h>

#define MAX_ITERATIONS 1000
#define EPSILON 1e-10
#define SQUARE(x) ((x) * (x))
#define ABS(x) ((x) < 0 ? -(x) : (x))
#define IS_ZERO(x) (ABS(x) < EPSILON)

double compute_polynomial(double x, double coeffs[], int degree) {
    double result = 0.0;
    double term = 1.0;
    for (int i = 0; i <= degree; i++) {
        result += coeffs[i] * term;
        term *= x;
    }
    return result;
}

double compute_derivative(double x, double coeffs[], int degree) {
    double result = 0.0;
    double term = 1.0;
    for (int i = 1; i <= degree; i++) {
        result += i * coeffs[i] * term;
        term *= x;
    }
    return result;
}

int main() {
    int degree;
    
    if (scanf("%d", &degree) != 1) {
        fprintf(stderr, "Error: Invalid input for degree\n");
        return EXIT_FAILURE;
    }
    
    if (degree < 0 || degree > 10) {
        fprintf(stderr, "Error: Degree must be between 0 and 10\n");
        return EXIT_FAILURE;
    }
    
    double coeffs[11];
    for (int i = 0; i <= degree; i++) {
        if (scanf("%lf", &coeffs[i]) != 1) {
            fprintf(stderr, "Error: Invalid input for coefficient %d\n", i);
            return EXIT_FAILURE;
        }
    }
    
    double initial_guess;
    if (scanf("%lf", &initial_guess) != 1) {
        fprintf(stderr, "Error: Invalid input for initial guess\n");
        return EXIT_FAILURE;
    }
    
    if (!isfinite(initial_guess)) {
        fprintf(stderr, "Error: Initial guess must be finite\n");
        return EXIT_FAILURE;
    }
    
    double x = initial_guess;
    int iterations = 0;
    
    while (iterations < MAX_ITERATIONS) {
        double fx = compute_polynomial(x, coeffs, degree);
        if (IS_ZERO(fx)) {
            break;
        }
        
        double fpx = compute_derivative(x, coeffs, degree);
        if (IS_ZERO(fpx)) {
            fprintf(stderr, "Error: Derivative is zero, cannot continue\n");
            return EXIT_FAILURE;
        }
        
        double delta = fx / fpx;
        if (!isfinite(delta)) {
            fprintf(stderr, "Error: Numerical instability detected\n");
            return EXIT_FAILURE;
        }
        
        x = x - delta;
        iterations++;
        
        if (IS_ZERO(delta)) {
            break;
        }
    }
    
    if (iterations >= MAX_ITERATIONS) {
        fprintf(stderr, "Warning: Maximum iterations reached\n");
    }
    
    printf("Root: %.10f\n", x);
    printf("Iterations: %d\n", iterations);
    printf("Final value: %.10f\n", compute_polynomial(x, coeffs, degree));
    
    return EXIT_SUCCESS;
}