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
#define ABS(x) ((x) < 0 ? -(x) : (x))
#define SQUARE(x) ((x) * (x))

double newton_raphson_sqrt(double n) {
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
    double number;
    printf("Enter a number to compute its square root (range: %.1f to %.1f): ", MIN_INPUT, MAX_INPUT);
    
    if (scanf("%lf", &number) != 1) {
        printf("Error: Invalid input format\n");
        return 1;
    }
    
    if (!VALIDATE_INPUT(number)) {
        printf("Error: Input out of valid range\n");
        return 1;
    }
    
    double sqrt_result = newton_raphson_sqrt(number);
    if (sqrt_result < 0) {
        printf("Error: Cannot compute square root of negative number\n");
        return 1;
    }
    
    printf("Square root of %.6f is approximately %.10f\n", number, sqrt_result);
    
    double coefficients[] = {2.0, -3.0, 1.0, 0.5};
    int degree = 3;
    double poly_result = compute_polynomial(number, coefficients, degree);
    
    printf("Polynomial 2 - 3x + x^2 + 0.5x^3 evaluated at %.6f: %.10f\n", number, poly_result);
    
    double combined = sqrt_result + poly_result;
    printf("Combined result (sqrt + polynomial): %.10f\n", combined);
    
    double normalized = combined / (ABS(combined) + 1.0);
    printf("Normalized result: %.10f\n", normalized);
    
    return 0;
}