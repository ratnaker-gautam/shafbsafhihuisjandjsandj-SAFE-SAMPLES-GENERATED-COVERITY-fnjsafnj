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
    double prev;
    int iterations = 0;
    
    do {
        prev = x;
        x = 0.5 * (x + n / x);
        iterations++;
    } while (ABS(x - prev) > EPSILON && iterations < MAX_ITERATIONS);
    
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
        fprintf(stderr, "Error: Invalid input format\n");
        return EXIT_FAILURE;
    }
    
    if (!VALIDATE_INPUT(input)) {
        fprintf(stderr, "Error: Input out of valid range\n");
        return EXIT_FAILURE;
    }
    
    double sqrt_result = newton_sqrt(input);
    if (sqrt_result < 0) {
        fprintf(stderr, "Error: Cannot compute square root of negative number\n");
        return EXIT_FAILURE;
    }
    
    double coefficients[] = {2.0, -3.0, 1.5, -0.5};
    int degree = 3;
    double poly_result = compute_polynomial(input, coefficients, degree);
    
    printf("Input: %.6f\n", input);
    printf("Square root: %.6f\n", sqrt_result);
    printf("Polynomial f(x) = 2 - 3x + 1.5x^2 - 0.5x^3\n");
    printf("Polynomial result: %.6f\n", poly_result);
    
    double combined = sqrt_result + poly_result;
    if (!isfinite(combined)) {
        fprintf(stderr, "Error: Result is not finite\n");
        return EXIT_FAILURE;
    }
    
    printf("Combined result (sqrt + polynomial): %.6f\n", combined);
    
    return EXIT_SUCCESS;
}