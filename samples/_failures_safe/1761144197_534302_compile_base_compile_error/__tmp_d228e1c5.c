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
        if (x == 0.0) break;
        x = 0.5 * (x + n / x);
        iterations++;
        if (iterations > MAX_ITERATIONS) break;
    } while (ABS(x - prev) > EPSILON);
    
    return x;
}

double compute_power(double base, int exponent) {
    if (exponent == 0) return 1.0;
    if (base == 1.0) return 1.0;
    if (base == -1.0) return (exponent % 2 == 0) ? 1.0 : -1.0;
    
    if (exponent < 0) {
        if (base == 0.0) return 0.0;
        return 1.0 / compute_power(base, -exponent);
    }
    
    double result = 1.0;
    for (int i = 0; i < exponent; i++) {
        if (ABS(result) > DBL_MAX / ABS(base)) {
            if ((base > 0 && result > 0) || (base < 0 && result < 0)) {
                return DBL_MAX;
            } else {
                return -DBL_MAX;
            }
        }
        result *= base;
    }
    return result;
}

int main() {
    double number;
    int choice;
    
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
    printf("1. Square root\n");
    printf("2. Square\n");
    printf("3. Cube\n");
    printf("4. Natural logarithm\n");
    printf("Enter choice (1-4): ");
    
    if (scanf("%d", &choice) != 1) {
        printf("Invalid choice format.\n");
        return 1;
    }
    
    double result;
    
    switch (choice) {
        case 1:
            result = compute_sqrt_newton(number);
            if (result < 0) {
                printf("Cannot compute square root of negative number.\n");
                return 1;
            }
            printf("Square root: %.6f\n", result);
            break;
            
        case 2:
            result = compute_power(number, 2);
            if (result == DBL_MAX || result == -DBL_MAX) {
                printf("Result too large.\n");
                return 1;
            }
            printf("Square: %.6f\n", result);
            break;
            
        case 3:
            result = compute_power(number, 3);
            if (result == DBL_MAX || result == -DBL_MAX) {
                printf("Result too large.\n");
                return 1;
            }
            printf("Cube: %.6f\n", result);
            break;
            
        case 4:
            if (number <= 0) {
                printf("Cannot compute logarithm of non-positive number.\n");
                return 1;
            }
            result = log(number);
            printf("Natural logarithm: %.6f\n", result);
            break;
            
        default:
            printf("Invalid choice.\n");
            return 1;
    }
    
    return 0;
}