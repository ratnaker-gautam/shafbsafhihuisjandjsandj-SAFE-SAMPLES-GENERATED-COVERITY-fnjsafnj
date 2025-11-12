//DeepSeek-V3 V2.5 Category: Safe ; Style: modular_functions ; Variation: numeric_computation
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#define MAX_ITERATIONS 1000
#define EPSILON 1e-10

double compute_polynomial(double x, double coefficients[], int degree) {
    double result = 0.0;
    double term = 1.0;
    for (int i = 0; i <= degree; i++) {
        result += coefficients[i] * term;
        term *= x;
    }
    return result;
}

double compute_derivative(double x, double coefficients[], int degree) {
    double result = 0.0;
    double term = 1.0;
    for (int i = 1; i <= degree; i++) {
        result += i * coefficients[i] * term;
        term *= x;
    }
    return result;
}

int validate_coefficients(double coefficients[], int degree) {
    if (degree < 0 || degree > 10) {
        return 0;
    }
    for (int i = 0; i <= degree; i++) {
        if (!isfinite(coefficients[i])) {
            return 0;
        }
    }
    return 1;
}

double newton_raphson(double coefficients[], int degree, double initial_guess) {
    if (!validate_coefficients(coefficients, degree)) {
        return NAN;
    }
    
    double x = initial_guess;
    int iterations = 0;
    
    while (iterations < MAX_ITERATIONS) {
        double fx = compute_polynomial(x, coefficients, degree);
        double fpx = compute_derivative(x, coefficients, degree);
        
        if (fabs(fx) < EPSILON) {
            return x;
        }
        
        if (fabs(fpx) < EPSILON) {
            return NAN;
        }
        
        double delta = fx / fpx;
        if (!isfinite(delta)) {
            return NAN;
        }
        
        x = x - delta;
        
        if (!isfinite(x)) {
            return NAN;
        }
        
        iterations++;
    }
    
    return NAN;
}

int main() {
    double coefficients[11] = {0};
    int degree;
    double initial_guess;
    
    printf("Enter polynomial degree (0-10): ");
    if (scanf("%d", &degree) != 1) {
        printf("Invalid input for degree.\n");
        return 1;
    }
    
    if (degree < 0 || degree > 10) {
        printf("Degree must be between 0 and 10.\n");
        return 1;
    }
    
    printf("Enter coefficients from constant term to highest degree:\n");
    for (int i = 0; i <= degree; i++) {
        printf("Coefficient for x^%d: ", i);
        if (scanf("%lf", &coefficients[i]) != 1) {
            printf("Invalid input for coefficient.\n");
            return 1;
        }
    }
    
    printf("Enter initial guess: ");
    if (scanf("%lf", &initial_guess) != 1) {
        printf("Invalid input for initial guess.\n");
        return 1;
    }
    
    if (!isfinite(initial_guess)) {
        printf("Initial guess must be a finite number.\n");
        return 1;
    }
    
    double root = newton_raphson(coefficients, degree, initial_guess);
    
    if (isnan(root)) {
        printf("Failed to find a root with the given parameters.\n");
    } else {
        printf("Approximate root: %.10f\n", root);
        printf("Function value at root: %.10f\n", compute_polynomial(root, coefficients, degree));
    }
    
    return 0;
}