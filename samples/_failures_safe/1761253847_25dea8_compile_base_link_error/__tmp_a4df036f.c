//DeepSeek-V3 V2.5 Category: Safe ; Style: macros ; Variation: numeric_computation
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <stdint.h>
#include <limits.h>

#define MAX_ITERATIONS 1000
#define TOLERANCE 1e-10
#define MAX_INPUT 1000000.0
#define MIN_INPUT 1e-10

double safe_sqrt(double x) {
    if (x < 0.0) {
        return NAN;
    }
    return sqrt(x);
}

double safe_log(double x) {
    if (x <= 0.0) {
        return NAN;
    }
    return log(x);
}

double compute_function(double x) {
    if (isnan(x) || isinf(x)) {
        return NAN;
    }
    double term1 = safe_sqrt(x);
    if (isnan(term1)) {
        return NAN;
    }
    double term2 = safe_log(x + 1.0);
    if (isnan(term2)) {
        return NAN;
    }
    double result = term1 * term2;
    if (isnan(result) || isinf(result)) {
        return NAN;
    }
    return result;
}

double compute_derivative(double x) {
    if (isnan(x) || isinf(x) || x <= -1.0) {
        return NAN;
    }
    double sqrt_x = safe_sqrt(x);
    if (isnan(sqrt_x)) {
        return NAN;
    }
    double log_term = safe_log(x + 1.0);
    if (isnan(log_term)) {
        return NAN;
    }
    double derivative = (log_term / (2.0 * sqrt_x)) + (sqrt_x / (x + 1.0));
    if (isnan(derivative) || isinf(derivative)) {
        return NAN;
    }
    return derivative;
}

int main(void) {
    double initial_guess;
    printf("Enter initial guess for root finding (1e-10 to 1000000): ");
    if (scanf("%lf", &initial_guess) != 1) {
        fprintf(stderr, "Error: Invalid input\n");
        return EXIT_FAILURE;
    }
    if (initial_guess < MIN_INPUT || initial_guess > MAX_INPUT) {
        fprintf(stderr, "Error: Input out of valid range\n");
        return EXIT_FAILURE;
    }
    double x = initial_guess;
    int iterations = 0;
    while (iterations < MAX_ITERATIONS) {
        double fx = compute_function(x);
        if (isnan(fx) || isinf(fx)) {
            fprintf(stderr, "Error: Function evaluation failed\n");
            return EXIT_FAILURE;
        }
        if (fabs(fx) < TOLERANCE) {
            printf("Root found: %.10f\n", x);
            printf("Iterations: %d\n", iterations);
            return EXIT_SUCCESS;
        }
        double dfx = compute_derivative(x);
        if (isnan(dfx) || isinf(dfx) || fabs(dfx) < 1e-15) {
            fprintf(stderr, "Error: Derivative evaluation failed\n");
            return EXIT_FAILURE;
        }
        double x_new = x - fx / dfx;
        if (isnan(x_new) || isinf(x_new)) {
            fprintf(stderr, "Error: Newton step produced invalid value\n");
            return EXIT_FAILURE;
        }
        if (fabs(x_new - x) < TOLERANCE) {
            printf("Root found: %.10f\n", x_new);
            printf("Iterations: %d\n", iterations);
            return EXIT_SUCCESS;
        }
        x = x_new;
        iterations++;
    }
    fprintf(stderr, "Error: Maximum iterations reached\n");
    return EXIT_FAILURE;
}