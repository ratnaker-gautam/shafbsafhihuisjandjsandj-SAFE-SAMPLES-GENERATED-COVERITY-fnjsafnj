//DeepSeek-V3 V2.5 Category: Safe ; Style: modular_functions ; Variation: numeric_computation
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#define MAX_ITERATIONS 1000
#define EPSILON 1e-10

double compute_polynomial(double x, double* coeffs, int degree) {
    if (coeffs == NULL || degree < 0) {
        return NAN;
    }
    
    double result = 0.0;
    double term = 1.0;
    
    for (int i = 0; i <= degree; i++) {
        result += coeffs[i] * term;
        term *= x;
    }
    
    return result;
}

double compute_derivative(double x, double* coeffs, int degree) {
    if (coeffs == NULL || degree < 1) {
        return NAN;
    }
    
    double result = 0.0;
    double term = 1.0;
    
    for (int i = 1; i <= degree; i++) {
        result += i * coeffs[i] * term;
        term *= x;
    }
    
    return result;
}

int find_polynomial_root(double* coeffs, int degree, double initial_guess, double* root) {
    if (coeffs == NULL || degree < 1 || root == NULL) {
        return -1;
    }
    
    double x = initial_guess;
    double fx, dfx;
    int iterations = 0;
    
    while (iterations < MAX_ITERATIONS) {
        fx = compute_polynomial(x, coeffs, degree);
        dfx = compute_derivative(x, coeffs, degree);
        
        if (fabs(dfx) < EPSILON) {
            return -2;
        }
        
        double delta = fx / dfx;
        x = x - delta;
        
        if (fabs(delta) < EPSILON) {
            *root = x;
            return iterations;
        }
        
        iterations++;
    }
    
    return -3;
}

int read_polynomial_coefficients(double** coeffs, int* degree) {
    if (coeffs == NULL || degree == NULL) {
        return -1;
    }
    
    printf("Enter polynomial degree (1-10): ");
    if (scanf("%d", degree) != 1) {
        return -1;
    }
    
    if (*degree < 1 || *degree > 10) {
        return -1;
    }
    
    *coeffs = malloc((*degree + 1) * sizeof(double));
    if (*coeffs == NULL) {
        return -1;
    }
    
    printf("Enter %d coefficients (from constant term to highest degree):\n", *degree + 1);
    for (int i = 0; i <= *degree; i++) {
        if (scanf("%lf", &(*coeffs)[i]) != 1) {
            free(*coeffs);
            return -1;
        }
    }
    
    return 0;
}

int main(void) {
    double* coefficients = NULL;
    int degree;
    double initial_guess, root;
    
    printf("Polynomial Root Finder using Newton-Raphson Method\n");
    printf("=================================================\n\n");
    
    if (read_polynomial_coefficients(&coefficients, &degree) != 0) {
        printf("Error: Invalid polynomial input.\n");
        return 1;
    }
    
    printf("Enter initial guess: ");
    if (scanf("%lf", &initial_guess) != 1) {
        printf("Error: Invalid initial guess.\n");
        free(coefficients);
        return 1;
    }
    
    int result = find_polynomial_root(coefficients, degree, initial_guess, &root);
    
    if (result >= 0) {
        printf("Root found at x = %.10f\n", root);
        printf("Converged in %d iterations\n", result);
        printf("Function value at root: %.10f\n", compute_polynomial(root, coefficients, degree));
    } else if (result == -1) {
        printf("Error: Invalid parameters.\n");
    } else if (result == -2) {
        printf("Error: Derivative too small, cannot continue.\n");
    } else if (result == -3) {
        printf("Error: Maximum iterations reached, no convergence.\n");
    }
    
    free(coefficients);
    return 0;
}