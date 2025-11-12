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

#define VALIDATE_INPUT(x) ((x) >= MIN_INPUT && (x) <= MAX_INPUT)
#define ABS(x) ((x) < 0 ? -(x) : (x))

double compute_sqrt_newton(double n) {
    if (n < 0) return NAN;
    if (n == 0) return 0.0;
    
    double x = n;
    double prev;
    int iter = 0;
    
    do {
        prev = x;
        if (x == 0.0) return NAN;
        x = 0.5 * (x + n / x);
        iter++;
    } while (ABS(x - prev) > EPSILON && iter < MAX_ITERATIONS);
    
    return x;
}

double compute_power(double base, int exponent) {
    if (exponent == 0) return 1.0;
    if (exponent < 0) {
        if (base == 0.0) return INFINITY;
        return 1.0 / compute_power(base, -exponent);
    }
    
    double result = 1.0;
    for (int i = 0; i < exponent; i++) {
        if (ABS(base) > 1.0 && result > DBL_MAX / ABS(base)) return INFINITY;
        if (ABS(base) < 1.0 && result < DBL_MIN / ABS(base)) return 0.0;
        result *= base;
    }
    return result;
}

int main() {
    double input_num;
    int exponent;
    
    printf("Enter a number between %.0f and %.0f: ", MIN_INPUT, MAX_INPUT);
    if (scanf("%lf", &input_num) != 1) {
        printf("Invalid input format.\n");
        return EXIT_FAILURE;
    }
    
    if (!VALIDATE_INPUT(input_num)) {
        printf("Input out of valid range.\n");
        return EXIT_FAILURE;
    }
    
    printf("Enter an integer exponent (-100 to 100): ");
    if (scanf("%d", &exponent) != 1) {
        printf("Invalid exponent format.\n");
        return EXIT_FAILURE;
    }
    
    if (exponent < -100 || exponent > 100) {
        printf("Exponent out of range.\n");
        return EXIT_FAILURE;
    }
    
    double sqrt_result = compute_sqrt_newton(input_num);
    double power_result = compute_power(input_num, exponent);
    
    printf("Square root: ");
    if (isnan(sqrt_result)) {
        printf("Undefined for negative numbers\n");
    } else {
        printf("%.6f\n", sqrt_result);
    }
    
    printf("Number^exponent: ");
    if (isinf(power_result)) {
        printf("Overflow occurred\n");
    } else if (power_result == 0.0 && input_num != 0.0) {
        printf("Underflow occurred\n");
    } else {
        printf("%.6f\n", power_result);
    }
    
    return EXIT_SUCCESS;
}