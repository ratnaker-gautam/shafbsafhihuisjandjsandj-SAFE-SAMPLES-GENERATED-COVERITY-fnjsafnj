//DeepSeek-V3 SAFE v1.4 Category: Safe ; Style: macros ; Variation: numeric_computation
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <limits.h>

#define MAX_ITERATIONS 1000
#define EPSILON 1e-10
#define SQUARE(x) ((x) * (x))
#define ABS(x) ((x) < 0 ? -(x) : (x))
#define IS_POSITIVE(x) ((x) > 0.0)
#define CLAMP(x, min, max) ((x) < (min) ? (min) : ((x) > (max) ? (max) : (x)))

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
    double coefficients[6];
    int degree;
    
    printf("Enter polynomial degree (1-5): ");
    if (scanf("%d", &degree) != 1) {
        fprintf(stderr, "Error: Invalid input for degree\n");
        return EXIT_FAILURE;
    }
    
    if (degree < 1 || degree > 5) {
        fprintf(stderr, "Error: Degree must be between 1 and 5\n");
        return EXIT_FAILURE;
    }
    
    printf("Enter %d coefficients (from constant to highest degree): ", degree + 1);
    for (int i = 0; i <= degree; i++) {
        if (scanf("%lf", &coefficients[i]) != 1) {
            fprintf(stderr, "Error: Invalid coefficient input\n");
            return EXIT_FAILURE;
        }
    }
    
    double initial_guess;
    printf("Enter initial guess: ");
    if (scanf("%lf", &initial_guess) != 1) {
        fprintf(stderr, "Error: Invalid initial guess\n");
        return EXIT_FAILURE;
    }
    
    double x = initial_guess;
    double prev_x;
    int iterations = 0;
    
    while (iterations < MAX_ITERATIONS) {
        double fx = compute_polynomial(x, coefficients, degree);
        double fpx = compute_derivative(x, coefficients, degree);
        
        if (ABS(fpx) < EPSILON) {
            fprintf(stderr, "Error: Derivative too small\n");
            return EXIT_FAILURE;
        }
        
        prev_x = x;
        x = x - fx / fpx;
        
        if (ABS(x - prev_x) < EPSILON) {
            break;
        }
        
        iterations++;
    }
    
    if (iterations >= MAX_ITERATIONS) {
        fprintf(stderr, "Error: Maximum iterations reached\n");
        return EXIT_FAILURE;
    }
    
    double final_value = compute_polynomial(x, coefficients, degree);
    
    printf("Root found at x = %.10f\n", x);
    printf("Polynomial value at root: %.10f\n", final_value);
    printf("Iterations required: %d\n", iterations);
    
    return EXIT_SUCCESS;
}