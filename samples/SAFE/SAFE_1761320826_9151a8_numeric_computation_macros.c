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

double compute_sqrt_newton(double n) {
    if (n < 0) return -1.0;
    if (n == 0.0) return 0.0;
    
    double x = n;
    double prev;
    int iterations = 0;
    
    do {
        prev = x;
        x = 0.5 * (x + n / x);
        iterations++;
        if (iterations > MAX_ITERATIONS) break;
    } while (ABS(x - prev) > EPSILON);
    
    return x;
}

double compute_power(double base, int exponent) {
    if (exponent == 0) return 1.0;
    if (exponent == 1) return base;
    
    double result = 1.0;
    int abs_exp = exponent < 0 ? -exponent : exponent;
    
    for (int i = 0; i < abs_exp; i++) {
        if (result > MAX_INPUT / ABS(base)) return -1.0;
        result *= base;
    }
    
    if (exponent < 0) {
        if (ABS(result) < EPSILON) return -1.0;
        result = 1.0 / result;
    }
    
    return result;
}

int main() {
    double number, base;
    int exponent;
    
    printf("Enter a number to compute square root: ");
    if (scanf("%lf", &number) != 1) {
        printf("Invalid input\n");
        return 1;
    }
    
    if (!VALIDATE_INPUT(number)) {
        printf("Input out of range\n");
        return 1;
    }
    
    double sqrt_result = compute_sqrt_newton(number);
    if (sqrt_result < 0) {
        printf("Cannot compute square root of negative number\n");
    } else {
        printf("Square root: %.6f\n", sqrt_result);
    }
    
    printf("Enter base for power computation: ");
    if (scanf("%lf", &base) != 1) {
        printf("Invalid input\n");
        return 1;
    }
    
    printf("Enter exponent (integer): ");
    if (scanf("%d", &exponent) != 1) {
        printf("Invalid input\n");
        return 1;
    }
    
    if (!VALIDATE_INPUT(base)) {
        printf("Base out of range\n");
        return 1;
    }
    
    if (exponent > 1000 || exponent < -1000) {
        printf("Exponent out of range\n");
        return 1;
    }
    
    double power_result = compute_power(base, exponent);
    if (power_result < 0) {
        printf("Power computation failed (overflow or division by zero)\n");
    } else {
        printf("Power result: %.6f\n", power_result);
    }
    
    return 0;
}