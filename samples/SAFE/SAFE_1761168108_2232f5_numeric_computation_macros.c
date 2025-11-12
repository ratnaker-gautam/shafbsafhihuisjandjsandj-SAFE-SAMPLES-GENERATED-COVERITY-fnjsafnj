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
    if (n < 0) return -1.0;
    if (n == 0.0) return 0.0;
    
    double x = n;
    for (int i = 0; i < MAX_ITERATIONS; i++) {
        double fx = SQUARE(x) - n;
        if (ABS(fx) < EPSILON) break;
        double fpx = 2.0 * x;
        if (ABS(fpx) < EPSILON) break;
        double delta = fx / fpx;
        x = x - delta;
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
    printf("Enter a number between %.1f and %.1f: ", MIN_INPUT, MAX_INPUT);
    
    if (scanf("%lf", &input) != 1) {
        printf("Error: Invalid input format\n");
        return EXIT_FAILURE;
    }
    
    if (!VALIDATE_INPUT(input)) {
        printf("Error: Input out of valid range\n");
        return EXIT_FAILURE;
    }
    
    double sqrt_result = newton_sqrt(input);
    if (sqrt_result < 0) {
        printf("Error: Cannot compute square root of negative number\n");
        return EXIT_FAILURE;
    }
    
    double coefficients[] = {2.0, -3.0, 1.5, -0.25};
    int degree = 3;
    double poly_result = compute_polynomial(input, coefficients, degree);
    
    printf("Square root: %.6f\n", sqrt_result);
    printf("Polynomial f(x) = 2 - 3x + 1.5x^2 - 0.25x^3 at x=%.2f: %.6f\n", input, poly_result);
    
    double combined = sqrt_result + poly_result;
    printf("Combined result (sqrt + polynomial): %.6f\n", combined);
    
    return EXIT_SUCCESS;
}