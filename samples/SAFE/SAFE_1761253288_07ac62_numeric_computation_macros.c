//DeepSeek-V3 V2.5 Category: Safe ; Style: macros ; Variation: numeric_computation
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <limits.h>
#include <float.h>

#define MAX_ITERATIONS 1000
#define EPSILON 1e-10
#define MAX_INPUT 1000000.0
#define MIN_INPUT -1000000.0

#define VALIDATE_INPUT(x) ((x) >= MIN_INPUT && (x) <= MAX_INPUT)
#define ABS(x) ((x) < 0 ? -(x) : (x))

double compute_sqrt_newton(double n) {
    if (n < 0) return -1.0;
    if (n == 0.0) return 0.0;
    
    double x = n;
    double prev;
    int iterations = 0;
    
    do {
        prev = x;
        if (x == 0.0) break;
        x = 0.5 * (x + n / x);
        iterations++;
        if (iterations > MAX_ITERATIONS) break;
    } while (ABS(x - prev) > EPSILON);
    
    return x;
}

double compute_power(double base, int exponent) {
    if (exponent == 0) return 1.0;
    if (base == 0.0) {
        if (exponent > 0) return 0.0;
        return 1.0 / 0.0;
    }
    
    if (exponent < 0) {
        if (base == 0.0) return 1.0 / 0.0;
        return 1.0 / compute_power(base, -exponent);
    }
    
    double result = 1.0;
    
    for (int i = 0; i < exponent; i++) {
        if (ABS(base) > 1.0 && result > DBL_MAX / ABS(base)) {
            return base > 0 ? DBL_MAX : -DBL_MAX;
        }
        if (ABS(base) < 1.0 && result < DBL_MIN / ABS(base)) {
            return 0.0;
        }
        result *= base;
    }
    
    return result;
}

int main(void) {
    double number;
    int exponent;
    
    printf("Enter a number between %.1f and %.1f: ", MIN_INPUT, MAX_INPUT);
    if (scanf("%lf", &number) != 1) {
        printf("Invalid input format.\n");
        return EXIT_FAILURE;
    }
    
    if (!VALIDATE_INPUT(number)) {
        printf("Input out of valid range.\n");
        return EXIT_FAILURE;
    }
    
    printf("Enter an integer exponent (-100 to 100): ");
    if (scanf("%d", &exponent) != 1) {
        printf("Invalid exponent format.\n");
        return EXIT_FAILURE;
    }
    
    if (exponent < -100 || exponent > 100) {
        printf("Exponent out of valid range.\n");
        return EXIT_FAILURE;
    }
    
    double sqrt_result = compute_sqrt_newton(number);
    if (sqrt_result < 0) {
        printf("Cannot compute square root of negative number.\n");
    } else {
        printf("Square root: %.10f\n", sqrt_result);
    }
    
    double power_result = compute_power(number, exponent);
    if (!isfinite(power_result)) {
        printf("Number raised to power %d: Overflow/Undefined\n", exponent);
    } else {
        printf("Number raised to power %d: %.10f\n", exponent, power_result);
    }
    
    double combined = compute_power(sqrt_result, exponent);
    if (!isfinite(combined)) {
        printf("Square root raised to power %d: Overflow/Undefined\n", exponent);
    } else {
        printf("Square root raised to power %d: %.10f\n", exponent, combined);
    }
    
    return EXIT_SUCCESS;
}