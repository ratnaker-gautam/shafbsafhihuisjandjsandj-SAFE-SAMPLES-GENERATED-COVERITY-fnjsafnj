//DeepSeek-V3 V2.5 Category: Safe ; Style: macros ; Variation: numeric_computation
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <stdint.h>
#include <limits.h>

#define MAX_ITERATIONS 1000
#define TOLERANCE 1e-10
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
    } while (ABS(x - prev) > TOLERANCE);
    
    return x;
}

double compute_factorial(int n) {
    if (n < 0) return -1.0;
    if (n == 0) return 1.0;
    
    double result = 1.0;
    for (int i = 1; i <= n; i++) {
        if (result > INT_MAX / i) {
            return -1.0;
        }
        result *= i;
    }
    return result;
}

double compute_power(double base, int exponent) {
    if (exponent < 0) return -1.0;
    if (exponent == 0) return 1.0;
    
    double result = 1.0;
    for (int i = 0; i < exponent; i++) {
        if (ABS(result) > INT_MAX / ABS(base)) {
            return -1.0;
        }
        result *= base;
    }
    return result;
}

int main() {
    double input_num;
    int choice;
    
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
    printf("2. Factorial (if integer)\n");
    printf("3. Square\n");
    printf("Enter choice (1-3): ");
    
    if (scanf("%d", &choice) != 1) {
        printf("Invalid choice format.\n");
        return 1;
    }
    
    double result;
    
    switch (choice) {
        case 1:
            result = compute_sqrt_newton(input_num);
            if (result < 0) {
                printf("Cannot compute square root of negative number.\n");
                return 1;
            }
            printf("Square root: %.10f\n", result);
            break;
            
        case 2:
            if (input_num != (int)input_num) {
                printf("Factorial requires integer input.\n");
                return 1;
            }
            result = compute_factorial((int)input_num);
            if (result < 0) {
                printf("Factorial computation failed (overflow or invalid input).\n");
                return 1;
            }
            printf("Factorial: %.0f\n", result);
            break;
            
        case 3:
            result = compute_power(input_num, 2);
            if (result < 0) {
                printf("Square computation failed (overflow).\n");
                return 1;
            }
            printf("Square: %.10f\n", result);
            break;
            
        default:
            printf("Invalid choice.\n");
            return 1;
    }
    
    return 0;
}