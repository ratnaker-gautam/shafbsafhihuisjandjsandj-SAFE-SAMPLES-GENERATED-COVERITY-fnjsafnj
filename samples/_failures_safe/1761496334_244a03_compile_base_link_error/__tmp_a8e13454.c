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
    for (int i = 0; i <= degree; i++) {
        result += coeffs[i] * pow(x, i);
    }
    return result;
}

double compute_derivative(double x, double coeffs[], int degree) {
    double result = 0.0;
    for (int i = 1; i <= degree; i++) {
        result += i * coeffs[i] * pow(x, i - 1);
    }
    return result;
}

int main(void) {
    double coefficients[4] = {2.0, -3.0, 1.5, -0.5};
    int degree = 3;
    double initial_guess;
    
    printf("Enter initial guess for root finding (-1000000 to 1000000): ");
    if (scanf("%lf", &initial_guess) != 1) {
        fprintf(stderr, "Error: Invalid input format\n");
        return EXIT_FAILURE;
    }
    
    if (!VALIDATE_INPUT(initial_guess)) {
        fprintf(stderr, "Error: Input out of valid range\n");
        return EXIT_FAILURE;
    }
    
    double x = initial_guess;
    double x_prev;
    int iterations = 0;
    
    printf("Finding root of polynomial using Newton-Raphson method...\n");
    
    do {
        x_prev = x;
        double fx = compute_polynomial(x, coefficients, degree);
        double fpx = compute_derivative(x, coefficients, degree);
        
        if (fabs(fpx) < EPSILON) {
            fprintf(stderr, "Error: Derivative too small, method fails\n");
            return EXIT_FAILURE;
        }
        
        x = x - fx / fpx;
        iterations++;
        
        if (iterations > MAX_ITERATIONS) {
            fprintf(stderr, "Error: Maximum iterations reached\n");
            return EXIT_FAILURE;
        }
        
    } while (fabs(x - x_prev) > EPSILON && iterations < MAX_ITERATIONS);
    
    printf("Root found: %.10f\n", x);
    printf("Function value at root: %.10f\n", compute_polynomial(x, coefficients, degree));
    printf("Iterations required: %d\n", iterations);
    
    return EXIT_SUCCESS;
}