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
    int iter = 0;
    
    do {
        prev = x;
        x = 0.5 * (x + n / x);
        iter++;
    } while (ABS(x - prev) > EPSILON && iter < MAX_ITERATIONS);
    
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
    double coefficients[4] = {2.0, -3.0, 1.5, -0.25};
    int degree = 3;
    
    printf("Enter a number between %.1f and %.1f: ", MIN_INPUT, MAX_INPUT);
    if (scanf("%lf", &input) != 1) {
        printf("Error: Invalid input format\n");
        return 1;
    }
    
    if (!VALIDATE_INPUT(input)) {
        printf("Error: Input out of valid range\n");
        return 1;
    }
    
    double sqrt_result = newton_sqrt(ABS(input));
    if (sqrt_result < 0) {
        printf("Error: Cannot compute square root of negative number\n");
        return 1;
    }
    
    double poly_result = compute_polynomial(input, coefficients, degree);
    
    printf("Input: %.6f\n", input);
    printf("Square root of absolute value: %.6f\n", sqrt_result);
    printf("Polynomial result: %.6f\n", poly_result);
    printf("Combined computation: %.6f\n", sqrt_result + poly_result);
    
    return 0;
}