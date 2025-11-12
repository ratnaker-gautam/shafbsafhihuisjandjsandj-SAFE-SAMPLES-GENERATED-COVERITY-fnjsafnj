//DeepSeek-V3 V2.5 Category: Safe ; Style: macros ; Variation: numeric_computation
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <stdint.h>
#include <limits.h>
#include <float.h>

#define MAX_ITERATIONS 1000
#define TOLERANCE 1e-10
#define MAX_INPUT 1000000.0
#define MIN_INPUT 1e-10

double compute_sqrt_newton(double x) {
    if (x < 0.0) {
        return NAN;
    }
    if (x == 0.0) {
        return 0.0;
    }
    double guess = x / 2.0;
    for (int i = 0; i < MAX_ITERATIONS; i++) {
        if (guess == 0.0) {
            return 0.0;
        }
        double new_guess = 0.5 * (guess + x / guess);
        if (fabs(new_guess - guess) < TOLERANCE) {
            return new_guess;
        }
        guess = new_guess;
    }
    return guess;
}

double compute_power(double base, int exponent) {
    if (exponent < 0) {
        return NAN;
    }
    if (base == 0.0) {
        return 0.0;
    }
    double result = 1.0;
    for (int i = 0; i < exponent; i++) {
        if (result > DBL_MAX / base) {
            return INFINITY;
        }
        result *= base;
    }
    return result;
}

int main(void) {
    double input_value;
    int exponent;
    
    printf("Enter a positive number for square root calculation: ");
    if (scanf("%lf", &input_value) != 1) {
        fprintf(stderr, "Error: Invalid input for number\n");
        return EXIT_FAILURE;
    }
    
    if (input_value < 0.0 || input_value > MAX_INPUT) {
        fprintf(stderr, "Error: Input must be between 0 and %f\n", MAX_INPUT);
        return EXIT_FAILURE;
    }
    
    printf("Enter a non-negative integer for power calculation: ");
    if (scanf("%d", &exponent) != 1) {
        fprintf(stderr, "Error: Invalid input for exponent\n");
        return EXIT_FAILURE;
    }
    
    if (exponent < 0 || exponent > 100) {
        fprintf(stderr, "Error: Exponent must be between 0 and 100\n");
        return EXIT_FAILURE;
    }
    
    double sqrt_result = compute_sqrt_newton(input_value);
    double power_result = compute_power(input_value, exponent);
    
    if (isnan(sqrt_result)) {
        fprintf(stderr, "Error: Cannot compute square root of negative number\n");
        return EXIT_FAILURE;
    }
    
    if (isnan(power_result)) {
        fprintf(stderr, "Error: Cannot compute power with negative exponent\n");
        return EXIT_FAILURE;
    }
    
    printf("Square root of %.6f: %.10f\n", input_value, sqrt_result);
    printf("%.6f raised to power %d: %.10f\n", input_value, exponent, power_result);
    
    double builtin_sqrt = sqrt(input_value);
    printf("Built-in sqrt for comparison: %.10f\n", builtin_sqrt);
    printf("Difference between implementations: %.10e\n", fabs(sqrt_result - builtin_sqrt));
    
    return EXIT_SUCCESS;
}