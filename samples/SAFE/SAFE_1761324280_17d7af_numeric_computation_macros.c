//DeepSeek-V3 V2.5 Category: Safe ; Style: macros ; Variation: numeric_computation
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
#define IS_VALID(x) ((x) >= MIN_INPUT && (x) <= MAX_INPUT)

double compute_sqrt_newton(double n) {
    if (n < 0.0) return -1.0;
    if (n == 0.0) return 0.0;
    double x = n;
    double prev = 0.0;
    for (int i = 0; i < MAX_ITERATIONS; i++) {
        prev = x;
        if (x == 0.0) return 0.0;
        x = 0.5 * (x + n / x);
        if (fabs(x - prev) < EPSILON) break;
    }
    return x;
}

double compute_power(double base, int exponent) {
    if (exponent == 0) return 1.0;
    if (exponent < 0) {
        if (base == 0.0) return 0.0;
        return 1.0 / compute_power(base, -exponent);
    }
    double result = 1.0;
    for (int i = 0; i < exponent; i++) {
        if (fabs(base) > 1.0 && result > DBL_MAX / fabs(base)) {
            return (base > 0.0) ? DBL_MAX : -DBL_MAX;
        }
        result *= base;
    }
    return result;
}

int main() {
    double input_value;
    int power;
    
    printf("Enter a number between %.0f and %.0f: ", MIN_INPUT, MAX_INPUT);
    if (scanf("%lf", &input_value) != 1) {
        printf("Invalid input format.\n");
        return EXIT_FAILURE;
    }
    
    if (!IS_VALID(input_value)) {
        printf("Input out of valid range.\n");
        return EXIT_FAILURE;
    }
    
    printf("Enter an integer power (-100 to 100): ");
    if (scanf("%d", &power) != 1) {
        printf("Invalid power input.\n");
        return EXIT_FAILURE;
    }
    
    if (power < -100 || power > 100) {
        printf("Power out of valid range.\n");
        return EXIT_FAILURE;
    }
    
    double sqrt_result = compute_sqrt_newton(input_value);
    if (sqrt_result < 0.0) {
        printf("Cannot compute square root of negative number.\n");
    } else {
        printf("Square root: %.10f\n", sqrt_result);
    }
    
    double power_result = compute_power(input_value, power);
    printf("Number raised to power %d: %.10f\n", power, power_result);
    
    double combined = compute_power(sqrt_result, power);
    printf("Square root raised to power %d: %.10f\n", power, combined);
    
    return EXIT_SUCCESS;
}