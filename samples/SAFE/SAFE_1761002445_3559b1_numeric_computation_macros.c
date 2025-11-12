//DeepSeek-V3 SAFE v1.4 Category: Safe ; Style: macros ; Variation: numeric_computation
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <stdint.h>
#include <limits.h>
#include <float.h>

#define MAX_ITERATIONS 1000
#define EPSILON 1e-10
#define MAX_INPUT 1000000.0
#define MIN_INPUT -1000000.0

#define VALIDATE_INPUT(x) ((x) >= MIN_INPUT && (x) <= MAX_INPUT)
#define ABS(x) ((x) < 0 ? -(x) : (x))

double compute_sqrt_newton(double n) {
    if (n < 0) return NAN;
    if (n == 0) return 0.0;
    
    double x = n;
    double prev;
    int iterations = 0;
    
    do {
        prev = x;
        if (fabs(x) < EPSILON) return NAN;
        x = 0.5 * (x + n / x);
        iterations++;
    } while (ABS(x - prev) > EPSILON && iterations < MAX_ITERATIONS);
    
    return x;
}

double compute_power(double base, int exponent) {
    if (exponent == 0) return 1.0;
    if (exponent < 0) {
        if (fabs(base) < EPSILON) return INFINITY;
        return 1.0 / compute_power(base, -exponent);
    }
    
    double result = 1.0;
    for (int i = 0; i < exponent; i++) {
        if (result > DBL_MAX / ABS(base)) return INFINITY;
        result *= base;
    }
    return result;
}

int main() {
    double input_value;
    int operation;
    
    printf("Enter a number between %.0f and %.0f: ", MIN_INPUT, MAX_INPUT);
    if (scanf("%lf", &input_value) != 1) {
        printf("Invalid input format.\n");
        return EXIT_FAILURE;
    }
    
    if (!VALIDATE_INPUT(input_value)) {
        printf("Input out of valid range.\n");
        return EXIT_FAILURE;
    }
    
    printf("Choose operation:\n");
    printf("1. Square root\n");
    printf("2. Square\n");
    printf("3. Cube\n");
    printf("4. Reciprocal\n");
    printf("Enter choice (1-4): ");
    
    if (scanf("%d", &operation) != 1) {
        printf("Invalid operation choice.\n");
        return EXIT_FAILURE;
    }
    
    double result;
    
    switch (operation) {
        case 1:
            result = compute_sqrt_newton(input_value);
            if (isnan(result)) {
                printf("Cannot compute square root of negative number.\n");
                return EXIT_FAILURE;
            }
            printf("Square root: %.10f\n", result);
            break;
            
        case 2:
            result = compute_power(input_value, 2);
            if (isinf(result)) {
                printf("Result too large to represent.\n");
                return EXIT_FAILURE;
            }
            printf("Square: %.10f\n", result);
            break;
            
        case 3:
            result = compute_power(input_value, 3);
            if (isinf(result)) {
                printf("Result too large to represent.\n");
                return EXIT_FAILURE;
            }
            printf("Cube: %.10f\n", result);
            break;
            
        case 4:
            if (fabs(input_value) < EPSILON) {
                printf("Cannot compute reciprocal of zero.\n");
                return EXIT_FAILURE;
            }
            result = 1.0 / input_value;
            printf("Reciprocal: %.10f\n", result);
            break;
            
        default:
            printf("Invalid operation selected.\n");
            return EXIT_FAILURE;
    }
    
    return EXIT_SUCCESS;
}