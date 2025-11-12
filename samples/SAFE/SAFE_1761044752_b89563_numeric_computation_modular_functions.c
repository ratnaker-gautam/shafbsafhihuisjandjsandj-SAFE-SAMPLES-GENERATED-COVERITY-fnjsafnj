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

int find_polynomial_root(double* coefficients, int degree, double initial_guess, double* root) {
    if (degree < 1 || coefficients == NULL || root == NULL) {
        return -1;
    }
    
    double x = initial_guess;
    int iterations = 0;
    
    while (iterations < MAX_ITERATIONS) {
        double fx = compute_polynomial(x, coefficients, degree);
        double fpx = compute_derivative(x, coefficients, degree);
        
        if (fabs(fpx) < EPSILON) {
            return -2;
        }
        
        double x_new = x - fx / fpx;
        
        if (fabs(x_new - x) < EPSILON) {
            *root = x_new;
            return 0;
        }
        
        x = x_new;
        iterations++;
    }
    
    return -3;
}

int read_polynomial_coefficients(double** coefficients, int* degree) {
    if (coefficients == NULL || degree == NULL) {
        return -1;
    }
    
    printf("Enter polynomial degree (1-10): ");
    if (scanf("%d", degree) != 1) {
        return -2;
    }
    
    if (*degree < 1 || *degree > 10) {
        return -3;
    }
    
    *coefficients = malloc((*degree + 1) * sizeof(double));
    if (*coefficients == NULL) {
        return -4;
    }
    
    printf("Enter %d coefficients from constant term to highest degree:\n", *degree + 1);
    for (int i = 0; i <= *degree; i++) {
        if (scanf("%lf", &(*coefficients)[i]) != 1) {
            free(*coefficients);
            return -5;
        }
    }
    
    return 0;
}

int main() {
    double* coefficients = NULL;
    int degree = 0;
    double initial_guess = 0.0;
    double root = 0.0;
    
    int result = read_polynomial_coefficients(&coefficients, &degree);
    if (result != 0) {
        printf("Error reading polynomial coefficients.\n");
        return 1;
    }
    
    printf("Enter initial guess for root: ");
    if (scanf("%lf", &initial_guess) != 1) {
        printf("Invalid initial guess.\n");
        free(coefficients);
        return 1;
    }
    
    result = find_polynomial_root(coefficients, degree, initial_guess, &root);
    
    if (result == 0) {
        printf("Found root: %.10f\n", root);
        printf("Polynomial value at root: %.10f\n", compute_polynomial(root, coefficients, degree));
    } else if (result == -2) {
        printf("Error: Derivative too small near root.\n");
    } else if (result == -3) {
        printf("Error: Maximum iterations reached.\n");
    } else {
        printf("Error: Failed to find root.\n");
    }
    
    free(coefficients);
    return 0;
}