//DeepSeek-V3 SAFE v1.4 Category: Safe ; Style: modular_functions ; Variation: numeric_computation
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

double newton_raphson(double* coefficients, int degree, double initial_guess) {
    double x = initial_guess;
    double fx, dfx;
    int iterations = 0;
    
    while (iterations < MAX_ITERATIONS) {
        fx = compute_polynomial(x, coefficients, degree);
        dfx = compute_derivative(x, coefficients, degree);
        
        if (fabs(dfx) < EPSILON) {
            break;
        }
        
        double delta = fx / dfx;
        
        if (fabs(delta) < EPSILON) {
            break;
        }
        
        x = x - delta;
        iterations++;
    }
    
    return x;
}

int validate_coefficients(double* coefficients, int degree) {
    if (coefficients == NULL || degree < 0 || degree > 10) {
        return 0;
    }
    
    int non_zero_found = 0;
    for (int i = 0; i <= degree; i++) {
        if (!isfinite(coefficients[i])) {
            return 0;
        }
        if (coefficients[i] != 0.0) {
            non_zero_found = 1;
        }
    }
    
    return non_zero_found;
}

int main() {
    int degree;
    
    printf("Enter polynomial degree (0-10): ");
    if (scanf("%d", &degree) != 1) {
        printf("Invalid input\n");
        return 1;
    }
    
    if (degree < 0 || degree > 10) {
        printf("Degree must be between 0 and 10\n");
        return 1;
    }
    
    double coefficients[11] = {0};
    
    printf("Enter %d coefficients (from constant term to highest degree):\n", degree + 1);
    for (int i = 0; i <= degree; i++) {
        if (scanf("%lf", &coefficients[i]) != 1) {
            printf("Invalid coefficient\n");
            return 1;
        }
    }
    
    if (!validate_coefficients(coefficients, degree)) {
        printf("Invalid coefficients\n");
        return 1;
    }
    
    double initial_guess;
    printf("Enter initial guess: ");
    if (scanf("%lf", &initial_guess) != 1) {
        printf("Invalid initial guess\n");
        return 1;
    }
    
    if (!isfinite(initial_guess)) {
        printf("Initial guess must be finite\n");
        return 1;
    }
    
    double root = newton_raphson(coefficients, degree, initial_guess);
    
    printf("Approximate root: %.10f\n", root);
    printf("Function value at root: %.10f\n", compute_polynomial(root, coefficients, degree));
    
    return 0;
}