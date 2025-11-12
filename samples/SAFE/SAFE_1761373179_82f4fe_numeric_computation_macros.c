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
    double fx, dfx, delta;

    while (iterations < MAX_ITERATIONS) {
        fx = compute_polynomial(x, coeffs, degree);
        if (IS_ZERO(fx)) {
            break;
        }

        dfx = compute_derivative(x, coeffs, degree);
        if (IS_ZERO(dfx)) {
            printf("Derivative is zero, cannot continue\n");
            return 1;
        }

        delta = fx / dfx;
        if (IS_ZERO(delta)) {
            break;
        }

        x = x - delta;
        iterations++;
    }

    if (iterations >= MAX_ITERATIONS) {
        printf("Maximum iterations reached\n");
    } else {
        printf("Root found at x = %.10f\n", x);
        printf("f(x) = %.10f\n", compute_polynomial(x, coeffs, degree));
        printf("Iterations: %d\n", iterations);
    }

    return 0;
}