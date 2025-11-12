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
        if (x == 0) return 0.0;
        x = 0.5 * (x + n / x);
        iterations++;
    } while (ABS(x - prev) > EPSILON && iterations < MAX_ITERATIONS);
    
    return x;
}

double compute_log_approx(double n) {
    if (n <= 0) return NAN;
    
    double result = 0.0;
    double term = (n - 1) / (n + 1);
    double term_sq = term * term;
    double current_term = term;
    
    for (int i = 1; i <= 50; i++) {
        result += current_term / (2 * i - 1);
        current_term *= term_sq;
        if (ABS(current_term) < EPSILON) break;
    }
    
    return 2 * result;
}

double compute_power(double base, int exponent) {
    if (exponent == 0) return 1.0;
    if (base == 0) return 0.0;
    
    double result = 1.0;
    int abs_exp = ABS(exponent);
    double abs_base = ABS(base);
    
    for (int i = 0; i < abs_exp; i++) {
        if (result > DBL_MAX / abs_base) {
            return exponent > 0 ? INFINITY : 0.0;
        }
        result *= abs_base;
    }
    
    if (base < 0 && (exponent % 2 != 0)) {
        result = -result;
    }
    
    return exponent < 0 ? 1.0 / result : result;
}

int main() {
    double input1, input2;
    int choice;
    
    printf("Enter first number: ");
    if (scanf("%lf", &input1) != 1) {
        printf("Invalid input\n");
        return 1;
    }
    
    printf("Enter second number: ");
    if (scanf("%lf", &input2) != 1) {
        printf("Invalid input\n");
        return 1;
    }
    
    if (!VALIDATE_INPUT(input1) || !VALIDATE_INPUT(input2)) {
        printf("Input out of valid range [%g, %g]\n", MIN_INPUT, MAX_INPUT);
        return 1;
    }
    
    printf("Choose operation:\n");
    printf("1. Square root of first number\n");
    printf("2. Natural logarithm of first number\n");
    printf("3. First number raised to integer part of second number\n");
    printf("4. All operations\n");
    printf("Enter choice (1-4): ");
    
    if (scanf("%d", &choice) != 1) {
        printf("Invalid choice\n");
        return 1;
    }
    
    if (choice < 1 || choice > 4) {
        printf("Invalid choice\n");
        return 1;
    }
    
    if (choice == 1 || choice == 4) {
        double sqrt_result = compute_sqrt_newton(input1);
        if (isnan(sqrt_result)) {
            printf("Square root: Cannot compute for negative number\n");
        } else {
            printf("Square root: %.10g\n", sqrt_result);
        }
    }
    
    if (choice == 2 || choice == 4) {
        double log_result = compute_log_approx(input1);
        if (isnan(log_result)) {
            printf("Natural logarithm: Cannot compute for non-positive number\n");
        } else {
            printf("Natural logarithm: %.10g\n", log_result);
        }
    }
    
    if (choice == 3 || choice == 4) {
        int exponent = (int)input2;
        if (exponent > 1000 || exponent < -1000) {
            printf("Power: Exponent magnitude too large\n");
        } else {
            double power_result = compute_power(input1, exponent);
            if (isinf(power_result)) {
                printf("Power: Result overflow\n");
            } else if (power_result == 0.0 && input1 != 0.0) {
                printf("Power: Result underflow\n");
            } else {
                printf("Power: %.10g\n", power_result);
            }
        }
    }
    
    return 0;
}