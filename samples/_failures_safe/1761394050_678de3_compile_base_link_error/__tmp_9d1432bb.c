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
    
    int abs_exp = exponent < 0 ? -exponent : exponent;
    double result = 1.0;
    
    while (abs_exp > 0) {
        if (abs_exp & 1) {
            result *= base;
        }
        base *= base;
        abs_exp >>= 1;
    }
    
    return exponent < 0 ? 1.0 / result : result;
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
    printf("1. Square root (Newton's method)\n");
    printf("2. Cube root (approximation)\n");
    printf("3. Square\n");
    printf("4. Cube\n");
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
            printf("Square root: %.10f\n", result);
            printf("Library sqrt: %.10f\n", sqrt(number));
            break;
            
        case 2:
            if (number < 0) {
                result = -compute_power(-number, 1.0/3.0);
            } else {
                result = compute_power(number, 1.0/3.0);
            }
            printf("Cube root approximation: %.10f\n", result);
            printf("Library cbrt: %.10f\n", cbrt(number));
            break;
            
        case 3:
            result = compute_power(number, 2);
            printf("Square: %.10f\n", result);
            printf("Library pow: %.10f\n", pow(number, 2));
            break;
            
        case 4:
            result = compute_power(number, 3);
            printf("Cube: %.10f\n", result);
            printf("Library pow: %.10f\n", pow(number, 3));
            break;
            
        default:
            printf("Invalid choice. Must be 1-4.\n");
            return 1;
    }
    
    return 0;
}