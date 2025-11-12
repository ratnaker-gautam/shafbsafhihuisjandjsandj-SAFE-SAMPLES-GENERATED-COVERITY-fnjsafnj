//DeepSeek-V3 V2.5 Category: Safe ; Style: modular_functions ; Variation: numeric_computation
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#define MAX_ITERATIONS 1000
#define TOLERANCE 1e-10

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

int read_polynomial(double* coeffs, int* degree) {
    printf("Enter polynomial degree (1-10): ");
    if (scanf("%d", degree) != 1) {
        return 0;
    }
    if (*degree < 1 || *degree > 10) {
        return 0;
    }
    printf("Enter %d coefficients from highest to lowest degree: ", *degree + 1);
    for (int i = 0; i <= *degree; i++) {
        if (scanf("%lf", &coeffs[i]) != 1) {
            return 0;
        }
    }
    return 1;
}

double find_root_newton(double* coeffs, int degree, double initial_guess) {
    double x = initial_guess;
    double fx, dfx, delta;
    int iterations = 0;
    
    while (iterations < MAX_ITERATIONS) {
        fx = compute_polynomial(x, coeffs, degree);
        if (fabs(fx) < TOLERANCE) {
            return x;
        }
        dfx = compute_derivative(x, coeffs, degree);
        if (fabs(dfx) < TOLERANCE) {
            break;
        }
        delta = fx / dfx;
        if (fabs(delta) < TOLERANCE) {
            return x;
        }
        x = x - delta;
        iterations++;
    }
    return x;
}

int main() {
    double coeffs[11];
    int degree;
    double initial_guess;
    
    if (!read_polynomial(coeffs, &degree)) {
        printf("Invalid input.\n");
        return 1;
    }
    
    printf("Enter initial guess: ");
    if (scanf("%lf", &initial_guess) != 1) {
        printf("Invalid input.\n");
        return 1;
    }
    
    double root = find_root_newton(coeffs, degree, initial_guess);
    double value = compute_polynomial(root, coeffs, degree);
    
    printf("Approximate root: %.10f\n", root);
    printf("Polynomial value at root: %.10f\n", value);
    
    return 0;
}