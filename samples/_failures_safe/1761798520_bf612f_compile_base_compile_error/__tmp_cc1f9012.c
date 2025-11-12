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
    
    double guess = n / 2.0;
    for (int i = 0; i < MAX_ITERATIONS; i++) {
        if (guess == 0.0) return 0.0;
        double new_guess = 0.5 * (guess + n / guess);
        if (ABS(new_guess - guess) < EPSILON) {
            return new_guess;
        }
        guess = new_guess;
    }
    return guess;
}

double compute_power(double base, int exponent) {
    if (exponent == 0) return 1.0;
    if (base == 0.0) {
        if (exponent > 0) return 0.0;
        return 1.0 / 0.0;
    }
    
    if (exponent < 0) {
        base = 1.0 / base;
        exponent = -exponent;
    }
    
    double result = 1.0;
    for (int i = 0; i < exponent; i++) {
        if (ABS(base) > 1.0 && result > DBL_MAX / ABS(base)) {
            return base > 0 ? DBL_MAX : -DBL_MAX;
        }
        result *= base;
    }
    return result;
}

int main() {
    double input_num;
    int choice, exponent;
    
    printf("Enter a number between %.0f and %.0f: ", MIN_INPUT, MAX_INPUT);
    if (scanf("%lf", &input_num) != 1) {
        printf("Invalid input format.\n");
        return 1;
    }
    
    if (!VALIDATE_INPUT(input_num)) {
        printf("Input out of valid range.\n");
        return 1;
    }
    
    printf("Choose operation:\n");
    printf("1. Square root (Newton's method)\n");
    printf("2. Power (integer exponent)\n");
    if (scanf("%d", &choice) != 1) {
        printf("Invalid choice.\n");
        return 1;
    }
    
    switch (choice) {
        case 1: {
            double result = compute_sqrt_newton(input_num);
            if (result < 0) {
                printf("Cannot compute square root of negative number.\n");
                return 1;
            }
            printf("Square root: %.10f\n", result);
            printf("Library sqrt: %.10f\n", sqrt(input_num));
            break;
        }
        case 2: {
            printf("Enter integer exponent (-100 to 100): ");
            if (scanf("%d", &exponent) != 1) {
                printf("Invalid exponent.\n");
                return 1;
            }
            if (exponent < -100 || exponent > 100) {
                printf("Exponent out of range.\n");
                return 1;
            }
            double result = compute_power(input_num, exponent);
            printf("Power result: %.10f\n", result);
            break;
        }
        default:
            printf("Invalid operation choice.\n");
            return 1;
    }
    
    return 0;
}