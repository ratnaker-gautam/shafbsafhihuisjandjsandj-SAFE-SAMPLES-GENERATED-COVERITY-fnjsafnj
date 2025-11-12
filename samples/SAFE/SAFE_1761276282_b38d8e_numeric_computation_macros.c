//DeepSeek-V3 V2.5 Category: Safe ; Style: macros ; Variation: numeric_computation
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <limits.h>

#define MAX_ITERATIONS 1000
#define EPSILON 1e-10
#define IS_VALID(x) ((x) >= -1000.0 && (x) <= 1000.0)
#define CLAMP(x, min, max) (((x) < (min)) ? (min) : (((x) > (max)) ? (max) : (x)))

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
    printf("Enter polynomial degree (1-10): ");
    if (scanf("%d", &degree) != 1) {
        fprintf(stderr, "Error: Invalid input\n");
        return 1;
    }
    
    if (degree < 1 || degree > 10) {
        fprintf(stderr, "Error: Degree must be between 1 and 10\n");
        return 1;
    }
    
    double coeffs[11];
    printf("Enter %d coefficients (from constant to highest degree): ", degree + 1);
    for (int i = 0; i <= degree; i++) {
        if (scanf("%lf", &coeffs[i]) != 1) {
            fprintf(stderr, "Error: Invalid coefficient\n");
            return 1;
        }
    }
    
    double initial_guess;
    printf("Enter initial guess: ");
    if (scanf("%lf", &initial_guess) != 1) {
        fprintf(stderr, "Error: Invalid initial guess\n");
        return 1;
    }
    
    if (!IS_VALID(initial_guess)) {
        fprintf(stderr, "Error: Initial guess out of range\n");
        return 1;
    }
    
    double x = initial_guess;
    int iterations = 0;
    
    while (iterations < MAX_ITERATIONS) {
        double fx = compute_polynomial(x, coeffs, degree);
        if (fabs(fx) < EPSILON) {
            break;
        }
        
        double fpx = compute_derivative(x, coeffs, degree);
        if (fabs(fpx) < EPSILON) {
            fprintf(stderr, "Error: Derivative too small\n");
            return 1;
        }
        
        double delta = fx / fpx;
        if (!IS_VALID(delta)) {
            fprintf(stderr, "Error: Numerical instability\n");
            return 1;
        }
        
        double x_new = x - delta;
        x_new = CLAMP(x_new, -1000.0, 1000.0);
        
        if (fabs(x_new - x) < EPSILON) {
            x = x_new;
            break;
        }
        
        x = x_new;
        iterations++;
    }
    
    if (iterations >= MAX_ITERATIONS) {
        fprintf(stderr, "Error: Maximum iterations reached\n");
        return 1;
    }
    
    double final_value = compute_polynomial(x, coeffs, degree);
    if (fabs(final_value) > 1e-5) {
        fprintf(stderr, "Warning: Solution may not be accurate\n");
    }
    
    printf("Root found at x = %.10f\n", x);
    printf("f(x) = %.10f\n", final_value);
    printf("Iterations: %d\n", iterations);
    
    return 0;
}