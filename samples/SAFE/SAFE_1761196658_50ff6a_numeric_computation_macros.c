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
        if (ABS(x) < EPSILON) break;
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
        if (ABS(base) > 1.0 && result > DBL_MAX / ABS(base)) {
            return DBL_MAX;
        }
        result *= base;
    }
    
    if (exponent < 0) {
        if (ABS(result) < EPSILON) return DBL_MAX;
        return 1.0 / result;
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
    printf("4. Reciprocal\n");
    
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
            printf("Square root: %.10f\n", result);
            break;
            
        case 2:
            result = compute_power(number, 2);
            if (result == DBL_MAX) {
                printf("Result too large.\n");
                return 1;
            }
            printf("Square: %.10f\n", result);
            break;
            
        case 3:
            result = compute_power(number, 3);
            if (result == DBL_MAX) {
                printf("Result too large.\n");
                return 1;
            }
            printf("Cube: %.10f\n", result);
            break;
            
        case 4:
            if (ABS(number) < EPSILON) {
                printf("Cannot compute reciprocal of zero.\n");
                return 1;
            }
            result = compute_power(number, -1);
            if (result == DBL_MAX) {
                printf("Result too large.\n");
                return 1;
            }
            printf("Reciprocal: %.10f\n", result);
            break;
            
        default:
            printf("Invalid choice.\n");
            return 1;
    }
    
    return 0;
}