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
    double fx, fpx;
    int iterations = 0;
    
    while (iterations < MAX_ITERATIONS) {
        fx = compute_polynomial(x, coeffs, degree);
        fpx = compute_derivative(x, coeffs, degree);
        
        if (fabs(fpx) < EPSILON) {
            return NAN;
        }
        
        double delta = fx / fpx;
        
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

int get_valid_integer(const char* prompt, int min_val, int max_val) {
    int value;
    char buffer[100];
    
    while (1) {
        printf("%s", prompt);
        
        if (fgets(buffer, sizeof(buffer), stdin) == NULL) {
            return -1;
        }
        
        if (sscanf(buffer, "%d", &value) != 1) {
            printf("Invalid input. Please enter an integer.\n");
            continue;
        }
        
        if (value < min_val || value > max_val) {
            printf("Value must be between %d and %d.\n", min_val, max_val);
            continue;
        }
        
        break;
    }
    
    return value;
}

double get_valid_double(const char* prompt) {
    double value;
    char buffer[100];
    
    while (1) {
        printf("%s", prompt);
        
        if (fgets(buffer, sizeof(buffer), stdin) == NULL) {
            return NAN;
        }
        
        if (sscanf(buffer, "%lf", &value) != 1) {
            printf("Invalid input. Please enter a number.\n");
            continue;
        }
        
        if (!isfinite(value)) {
            printf("Value must be finite.\n");
            continue;
        }
        
        break;
    }
    
    return value;
}

int main() {
    printf("Polynomial Root Finder using Newton's Method\n");
    printf("===========================================\n\n");
    
    int degree = get_valid_integer("Enter polynomial degree (0-10): ", 0, 10);
    if (degree == -1) {
        return 1;
    }
    
    double coefficients[11];
    
    printf("Enter coefficients from constant term to highest degree:\n");
    for (int i = 0; i <= degree; i++) {
        char prompt[50];
        snprintf(prompt, sizeof(prompt), "Coefficient for x^%d: ", i);
        
        coefficients[i] = get_valid_double(prompt);
        if (!isfinite(coefficients[i])) {
            return 1;
        }
    }
    
    double initial_guess = get_valid_double("Enter initial guess: ");
    if (!isfinite(initial_guess)) {
        return 1;
    }
    
    double root = find_root_newton(coefficients, degree, initial_guess);
    
    printf("\nResults:\n");
    printf("--------\n");
    
    if (isnan(root)) {
        printf("Failed to find a root. The method may not converge.\n");
    } else {
        printf("Approximate root: %.10f\n", root);
        double function_value = compute_polynomial(root, coefficients, degree);
        printf("Function value at root: %.10f\n", function_value);
    }
    
    return 0;
}