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
    double coefficients[4] = {2.0, -3.0, 1.5, -0.5};
    int degree = 3;
    double initial_guess;
    int max_iterations;
    
    printf("Enter initial guess for root finding (-1000000 to 1000000): ");
    if (scanf("%lf", &initial_guess) != 1) {
        fprintf(stderr, "Error: Invalid input format\n");
        return EXIT_FAILURE;
    }
    
    if (!VALIDATE_INPUT(initial_guess)) {
        fprintf(stderr, "Error: Input out of valid range\n");
        return EXIT_FAILURE;
    }
    
    printf("Enter maximum iterations (1-1000): ");
    if (scanf("%d", &max_iterations) != 1) {
        fprintf(stderr, "Error: Invalid input format\n");
        return EXIT_FAILURE;
    }
    
    if (max_iterations < 1 || max_iterations > MAX_ITERATIONS) {
        fprintf(stderr, "Error: Iterations out of valid range\n");
        return EXIT_FAILURE;
    }
    
    double x = initial_guess;
    double fx, fpx;
    int iterations = 0;
    
    printf("\nNewton-Raphson Method Results:\n");
    printf("Iteration\tx\t\tf(x)\n");
    printf("----------------------------------------\n");
    
    while (iterations < max_iterations) {
        fx = compute_polynomial(x, coefficients, degree);
        fpx = compute_derivative(x, coefficients, degree);
        
        printf("%d\t\t%.6f\t%.6f\n", iterations, x, fx);
        
        if (fabs(fx) < EPSILON) {
            printf("\nRoot found at x = %.6f\n", x);
            printf("f(x) = %.6f\n", fx);
            return EXIT_SUCCESS;
        }
        
        if (fabs(fpx) < EPSILON) {
            fprintf(stderr, "Error: Derivative too small, method fails\n");
            return EXIT_FAILURE;
        }
        
        double delta = fx / fpx;
        if (fabs(delta) > 1e10) {
            fprintf(stderr, "Error: Method diverging\n");
            return EXIT_FAILURE;
        }
        
        x = x - delta;
        iterations++;
    }
    
    fprintf(stderr, "Error: Maximum iterations reached without convergence\n");
    return EXIT_FAILURE;
}