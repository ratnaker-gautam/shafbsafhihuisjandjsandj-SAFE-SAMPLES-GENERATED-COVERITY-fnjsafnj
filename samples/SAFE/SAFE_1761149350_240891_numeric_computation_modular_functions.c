//DeepSeek-V3 V2.5 Category: Safe ; Style: modular_functions ; Variation: numeric_computation
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#define MAX_ITERATIONS 1000
#define EPSILON 1e-10

double compute_polynomial(double x, double* coeffs, int degree) {
    double result = 0.0;
    double term = 1.0;
    
    for (int i = 0; i <= degree; i++) {
        result += coeffs[i] * term;
        term *= x;
    }
    
    return result;
}

double compute_derivative(double x, double* coeffs, int degree) {
    double result = 0.0;
    double term = 1.0;
    
    for (int i = 1; i <= degree; i++) {
        result += i * coeffs[i] * term;
        term *= x;
    }
    
    return result;
}

int validate_coefficients(double* coeffs, int degree) {
    if (degree < 0 || degree > 10) {
        return 0;
    }
    
    for (int i = 0; i <= degree; i++) {
        if (!isfinite(coeffs[i])) {
            return 0;
        }
    }
    
    return 1;
}

double find_root_newton(double* coeffs, int degree, double initial_guess) {
    if (!validate_coefficients(coeffs, degree)) {
        return NAN;
    }
    
    double x = initial_guess;
    int iterations = 0;
    
    while (iterations < MAX_ITERATIONS) {
        double fx = compute_polynomial(x, coeffs, degree);
        double fpx = compute_derivative(x, coeffs, degree);
        
        if (fabs(fpx) < EPSILON) {
            return NAN;
        }
        
        double delta = fx / fpx;
        
        if (fabs(delta) < EPSILON) {
            return x;
        }
        
        x -= delta;
        
        if (!isfinite(x)) {
            return NAN;
        }
        
        iterations++;
    }
    
    return NAN;
}

int get_user_input(double* coeffs, int* degree, double* initial_guess) {
    printf("Enter polynomial degree (0-10): ");
    if (scanf("%d", degree) != 1) {
        return 0;
    }
    
    if (*degree < 0 || *degree > 10) {
        return 0;
    }
    
    printf("Enter %d coefficients (from constant term to highest degree):\n", *degree + 1);
    for (int i = 0; i <= *degree; i++) {
        if (scanf("%lf", &coeffs[i]) != 1) {
            return 0;
        }
    }
    
    printf("Enter initial guess: ");
    if (scanf("%lf", initial_guess) != 1) {
        return 0;
    }
    
    return 1;
}

int main(void) {
    double coeffs[11];
    int degree;
    double initial_guess;
    
    printf("Polynomial Root Finder using Newton's Method\n");
    printf("============================================\n");
    
    if (!get_user_input(coeffs, &degree, &initial_guess)) {
        printf("Error: Invalid input\n");
        return 1;
    }
    
    if (!validate_coefficients(coeffs, degree)) {
        printf("Error: Invalid coefficients\n");
        return 1;
    }
    
    double root = find_root_newton(coeffs, degree, initial_guess);
    
    if (isnan(root)) {
        printf("No root found within %d iterations or method failed\n", MAX_ITERATIONS);
    } else {
        printf("Approximate root: %.10f\n", root);
        printf("Function value at root: %.10f\n", compute_polynomial(root, coeffs, degree));
    }
    
    return 0;
}