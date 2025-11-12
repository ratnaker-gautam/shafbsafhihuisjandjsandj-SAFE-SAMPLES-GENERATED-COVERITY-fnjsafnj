//DeepSeek-V3 V2.5 Category: Safe ; Style: macros ; Variation: numeric_computation
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
        fprintf(stderr, "Error: Invalid input\n");
        return EXIT_FAILURE;
    }
    
    if (degree < 1 || degree > 5) {
        fprintf(stderr, "Error: Degree must be between 1 and 5\n");
        return EXIT_FAILURE;
    }
    
    printf("Enter %d coefficients (from constant term to highest degree): ", degree + 1);
    for (int i = 0; i <= degree; i++) {
        if (scanf("%lf", &coefficients[i]) != 1) {
            fprintf(stderr, "Error: Invalid coefficient\n");
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
    
    printf("\nIteration  x          f(x)       f'(x)\n");
    printf("----------------------------------------\n");
    
    while (iterations < MAX_ITERATIONS) {
        double fx = compute_polynomial(x, coefficients, degree);
        double dfx = compute_derivative(x, coefficients, degree);
        
        printf("%9d  %10.6f  %10.6f  %10.6f\n", 
               iterations, x, fx, dfx);
        
        if (ABS(fx) < EPSILON) {
            printf("\nRoot found: x = %.10f\n", x);
            printf("f(x) = %.10f\n", fx);
            return EXIT_SUCCESS;
        }
        
        if (ABS(dfx) < EPSILON) {
            fprintf(stderr, "Error: Derivative too small\n");
            return EXIT_FAILURE;
        }
        
        prev_x = x;
        x = x - fx / dfx;
        
        if (ABS(x - prev_x) < EPSILON) {
            printf("\nConverged to root: x = %.10f\n", x);
            printf("f(x) = %.10f\n", compute_polynomial(x, coefficients, degree));
            return EXIT_SUCCESS;
        }
        
        iterations++;
    }
    
    fprintf(stderr, "Error: Maximum iterations reached\n");
    return EXIT_FAILURE;
}