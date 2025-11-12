//DeepSeek-V3 SAFE v1.4 Category: Safe ; Style: macros ; Variation: numeric_computation
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

double compute_sqrt_newton(double n) {
    if (n < 0) return NAN;
    if (n == 0) return 0.0;
    
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

double compute_power(double base, int exponent) {
    if (exponent == 0) return 1.0;
    if (exponent < 0) return 1.0 / compute_power(base, -exponent);
    
    double result = 1.0;
    while (exponent > 0) {
        if (exponent & 1) {
            result *= base;
        }
        base *= base;
        exponent >>= 1;
    }
    return result;
}

int main() {
    double input_value;
    int power;
    
    printf("Enter a number between %.0f and %.0f: ", MIN_INPUT, MAX_INPUT);
    if (scanf("%lf", &input_value) != 1) {
        printf("Error: Invalid input format.\n");
        return EXIT_FAILURE;
    }
    
    if (!VALIDATE_INPUT(input_value)) {
        printf("Error: Input out of valid range.\n");
        return EXIT_FAILURE;
    }
    
    printf("Enter an integer power (0-100): ");
    if (scanf("%d", &power) != 1) {
        printf("Error: Invalid power input.\n");
        return EXIT_FAILURE;
    }
    
    if (power < 0 || power > 100) {
        printf("Error: Power must be between 0 and 100.\n");
        return EXIT_FAILURE;
    }
    
    double sqrt_result = compute_sqrt_newton(input_value);
    double power_result = compute_power(input_value, power);
    
    if (isnan(sqrt_result)) {
        printf("Square root of %.2f is undefined (negative input).\n", input_value);
    } else {
        printf("Square root of %.2f: %.6f\n", input_value, sqrt_result);
    }
    
    printf("%.2f raised to power %d: %.6f\n", input_value, power, power_result);
    
    double verification = compute_power(sqrt_result, 2);
    if (!isnan(sqrt_result)) {
        printf("Verification (sqrt^2): %.6f\n", verification);
    }
    
    return EXIT_SUCCESS;
}