//DeepSeek-V3 V2.5 Category: Safe ; Style: modular_functions ; Variation: numeric_computation
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#define MAX_ITERATIONS 1000
#define EPSILON 1e-10

double compute_polynomial(double x, double* coefficients, int degree) {
    double result = 0.0;
    double term = 1.0;
    
    for (int i = 0; i <= degree; i++) {
        result += coefficients[i] * term;
        term *= x;
    }
    
    return result;
}

double compute_derivative(double x, double* coefficients, int degree) {
    double result = 0.0;
    double term = 1.0;
    
    for (int i = 1; i <= degree; i++) {
        result += i * coefficients[i] * term;
        term *= x;
    }
    
    return result;
}

int validate_input(double* coefficients, int degree, double initial_guess) {
    if (degree < 1 || degree > 10) {
        return 0;
    }
    
    if (coefficients == NULL) {
        return 0;
    }
    
    if (!isfinite(initial_guess)) {
        return 0;
    }
    
    for (int i = 0; i <= degree; i++) {
        if (!isfinite(coefficients[i])) {
            return 0;
        }
    }
    
    return 1;
}

double newton_raphson(double* coefficients, int degree, double initial_guess) {
    if (!validate_input(coefficients, degree, initial_guess)) {
        return NAN;
    }
    
    double x = initial_guess;
    double fx, dfx, delta;
    int iterations = 0;
    
    while (iterations < MAX_ITERATIONS) {
        fx = compute_polynomial(x, coefficients, degree);
        dfx = compute_derivative(x, coefficients, degree);
        
        if (fabs(dfx) < EPSILON) {
            return NAN;
        }
        
        delta = fx / dfx;
        
        if (fabs(delta) < EPSILON) {
            break;
        }
        
        x = x - delta;
        
        if (!isfinite(x)) {
            return NAN;
        }
        
        iterations++;
    }
    
    if (iterations >= MAX_ITERATIONS) {
        return NAN;
    }
    
    return x;
}

int main() {
    int degree;
    double initial_guess;
    
    printf("Enter polynomial degree (1-10): ");
    if (scanf("%d", &degree) != 1) {
        printf("Invalid input\n");
        return 1;
    }
    
    if (degree < 1 || degree > 10) {
        printf("Degree must be between 1 and 10\n");
        return 1;
    }
    
    double coefficients[11];
    
    printf("Enter coefficients from highest to lowest degree:\n");
    for (int i = degree; i >= 0; i--) {
        printf("Coefficient for x^%d: ", i);
        if (scanf("%lf", &coefficients[i]) != 1) {
            printf("Invalid input\n");
            return 1;
        }
    }
    
    printf("Enter initial guess: ");
    if (scanf("%lf", &initial_guess) != 1) {
        printf("Invalid input\n");
        return 1;
    }
    
    double root = newton_raphson(coefficients, degree, initial_guess);
    
    if (isnan(root)) {
        printf("Failed to find root within %d iterations\n", MAX_ITERATIONS);
        return 1;
    }
    
    printf("Approximate root: %.10f\n", root);
    printf("Function value at root: %.10f\n", compute_polynomial(root, coefficients, degree));
    
    return 0;
}