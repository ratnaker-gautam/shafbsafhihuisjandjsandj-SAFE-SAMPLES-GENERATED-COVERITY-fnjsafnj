//DeepSeek-V3 V2.5 Category: Safe ; Style: macros ; Variation: numeric_computation
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <stdint.h>
#include <limits.h>

#define MAX_ITERATIONS 1000
#define EPSILON 1e-10
#define MAX_INPUT 1000000.0
#define MIN_INPUT -1000000.0

#define VALIDATE_INPUT(x) ((x) >= MIN_INPUT && (x) <= MAX_INPUT)
#define SQUARE(x) ((x) * (x))
#define ABS(x) ((x) < 0 ? -(x) : (x))

double newton_sqrt(double n) {
    if (n < 0.0) {
        return -1.0;
    }
    if (n == 0.0) {
        return 0.0;
    }
    double x = n;
    for (int i = 0; i < MAX_ITERATIONS; i++) {
        double next_x = 0.5 * (x + n / x);
        if (ABS(next_x - x) < EPSILON) {
            return next_x;
        }
        x = next_x;
    }
    return x;
}

double compute_polynomial(double x, double coeffs[], int degree) {
    double result = 0.0;
    double term = 1.0;
    for (int i = 0; i <= degree; i++) {
        result += coeffs[i] * term;
        term *= x;
    }
    return result;
}

int main() {
    double input;
    printf("Enter a number between %.0f and %.0f: ", MIN_INPUT, MAX_INPUT);
    if (scanf("%lf", &input) != 1) {
        fprintf(stderr, "Error: Invalid input\n");
        return EXIT_FAILURE;
    }
    if (!VALIDATE_INPUT(input)) {
        fprintf(stderr, "Error: Input out of range\n");
        return EXIT_FAILURE;
    }
    double sqrt_val = newton_sqrt(input);
    if (sqrt_val < 0.0) {
        fprintf(stderr, "Error: Cannot compute square root of negative number\n");
        return EXIT_FAILURE;
    }
    double coefficients[] = {1.0, -2.0, 3.0, -4.0};
    int degree = 3;
    double poly_val = compute_polynomial(input, coefficients, degree);
    double combined = sqrt_val + poly_val;
    printf("Square root: %.6f\n", sqrt_val);
    printf("Polynomial value: %.6f\n", poly_val);
    printf("Combined result: %.6f\n", combined);
    return EXIT_SUCCESS;
}