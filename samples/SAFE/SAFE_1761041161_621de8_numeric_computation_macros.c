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

double compute_sqrt(double n) {
    if (n < 0) return -1.0;
    if (n == 0.0) return 0.0;
    
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

double compute_log(double n) {
    if (n <= 0) return -1.0;
    
    double result = 0.0;
    double term = (n - 1) / (n + 1);
    double term_sq = term * term;
    double current_term = term;
    int k = 1;
    
    while (ABS(current_term) > EPSILON && k < MAX_ITERATIONS) {
        result += current_term;
        current_term = current_term * term_sq * (2 * k - 1) / (2 * k + 1);
        k++;
    }
    
    return 2 * result;
}

double compute_power(double base, int exponent) {
    if (exponent == 0) return 1.0;
    if (base == 0.0) return 0.0;
    
    double result = 1.0;
    int abs_exp = exponent < 0 ? -exponent : exponent;
    
    for (int i = 0; i < abs_exp; i++) {
        if (result > MAX_INPUT / ABS(base)) {
            return exponent > 0 ? MAX_INPUT : 0.0;
        }
        result *= base;
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
    
    if (!VALIDATE_INPUT(input1)) {
        printf("Input out of range\n");
        return 1;
    }
    
    printf("Enter second number: ");
    if (scanf("%lf", &input2) != 1) {
        printf("Invalid input\n");
        return 1;
    }
    
    if (!VALIDATE_INPUT(input2)) {
        printf("Input out of range\n");
        return 1;
    }
    
    printf("Choose operation:\n");
    printf("1. Square root of first number\n");
    printf("2. Natural logarithm of first number\n");
    printf("3. First number raised to integer part of second number\n");
    printf("Enter choice (1-3): ");
    
    if (scanf("%d", &choice) != 1) {
        printf("Invalid choice\n");
        return 1;
    }
    
    switch (choice) {
        case 1: {
            double result = compute_sqrt(input1);
            if (result < 0) {
                printf("Cannot compute square root of negative number\n");
            } else {
                printf("Square root: %.10f\n", result);
            }
            break;
        }
        case 2: {
            double result = compute_log(input1);
            if (result < 0) {
                printf("Cannot compute logarithm of non-positive number\n");
            } else {
                printf("Natural logarithm: %.10f\n", result);
            }
            break;
        }
        case 3: {
            int exponent = (int)input2;
            if (exponent > 1000 || exponent < -1000) {
                printf("Exponent too large\n");
                break;
            }
            double result = compute_power(input1, exponent);
            printf("Power result: %.10f\n", result);
            break;
        }
        default:
            printf("Invalid choice\n");
            return 1;
    }
    
    return 0;
}