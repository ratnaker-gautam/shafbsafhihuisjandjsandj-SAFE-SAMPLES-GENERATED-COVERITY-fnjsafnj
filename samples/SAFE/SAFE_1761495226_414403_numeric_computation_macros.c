//DeepSeek-V3 V2.5 Category: Safe ; Style: macros ; Variation: numeric_computation
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <limits.h>

#define MAX_ITERATIONS 1000
#define EPSILON 1e-10
#define SQUARE(x) ((x) * (x))
#define ABS(x) ((x) < 0 ? -(x) : (x))
#define IS_ZERO(x) (ABS(x) < EPSILON)

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
    if (scanf("%d", &degree) != 1 || degree < 1 || degree > 10) {
        printf("Invalid degree\n");
        return 1;
    }

    double coeffs[11];
    printf("Enter %d coefficients from constant to highest degree: ", degree + 1);
    for (int i = 0; i <= degree; i++) {
        if (scanf("%lf", &coeffs[i]) != 1) {
            printf("Invalid coefficient\n");
            return 1;
        }
    }

    double initial_guess;
    printf("Enter initial guess: ");
    if (scanf("%lf", &initial_guess) != 1) {
        printf("Invalid guess\n");
        return 1;
    }

    double x = initial_guess;
    int iterations = 0;
    
    while (iterations < MAX_ITERATIONS) {
        double fx = compute_polynomial(x, coeffs, degree);
        if (IS_ZERO(fx)) {
            printf("Root found: %.10f\n", x);
            return 0;
        }
        
        double fpx = compute_derivative(x, coeffs, degree);
        if (IS_ZERO(fpx)) {
            printf("Derivative zero, cannot continue\n");
            return 1;
        }
        
        double delta = fx / fpx;
        if (IS_ZERO(delta)) {
            printf("Root found: %.10f\n", x);
            return 0;
        }
        
        double x_new = x - delta;
        if (IS_ZERO(x_new - x)) {
            printf("Root found: %.10f\n", x);
            return 0;
        }
        
        x = x_new;
        iterations++;
    }
    
    printf("Maximum iterations reached. Approximate root: %.10f\n", x);
    return 0;
}