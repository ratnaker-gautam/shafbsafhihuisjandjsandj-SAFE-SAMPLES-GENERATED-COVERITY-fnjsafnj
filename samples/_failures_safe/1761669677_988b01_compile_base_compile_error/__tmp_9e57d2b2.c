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
    if (n < 0) return -1.0;
    if (n == 0.0) return 0.0;
    
    double x = n;
    double prev;
    int iterations = 0;
    
    do {
        prev = x;
        if (x == 0.0) return 0.0;
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
        if (base != 0.0 && ABS(result) > DBL_MAX / ABS(base)) {
            return exponent < 0 ? 0.0 : DBL_MAX;
        }
        result *= base;
    }
    
    if (exponent < 0) {
        if (result == 0.0) return DBL_MAX;
        return 1.0 / result;
    }
    return result;
}

int main() {
    double number;
    int choice, exponent;
    
    printf("Enter a number between %.0f and %.0f: ", MIN_INPUT, MAX_INPUT);
    if (scanf("%lf", &number) != 1) {
        printf("Invalid input format.\n");
        return 1;
    }
    
    if (!VALIDATE_INPUT(number)) {
        printf("Input out of valid range.\n");
        return 1;
    }
    
    printf("Choose operation:\n");
    printf("1. Square root (Newton's method)\n");
    printf("2. Power (integer exponent)\n");
    printf("Enter choice (1 or 2): ");
    
    if (scanf("%d", &choice) != 1) {
        printf("Invalid choice format.\n");
        return 1;
    }
    
    switch (choice) {
        case 1: {
            double sqrt_result = compute_sqrt_newton(number);
            if (sqrt_result < 0) {
                printf("Cannot compute square root of negative number.\n");
                return 1;
            }
            printf("Square root of %.6f is approximately %.10f\n", number, sqrt_result);
            printf("Library sqrt: %.10f\n", sqrt(number));
            break;
        }
        case 2: {
            printf("Enter integer exponent (-100 to 100): ");
            if (scanf("%d", &exponent) != 1) {
                printf("Invalid exponent format.\n");
                return 1;
            }
            if (exponent < -100 || exponent > 100) {
                printf("Exponent out of range.\n");
                return 1;
            }
            double power_result = compute_power(number, exponent);
            printf("%.6f ^ %d = %.10f\n", number, exponent, power_result);
            break;
        }
        default:
            printf("Invalid choice.\n");
            return 1;
    }
    
    return 0;
}