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

double compute_log_approx(double n) {
    if (n <= 0) return -1.0;
    
    double result = 0.0;
    double term = (n - 1) / (n + 1);
    double term_sq = term * term;
    double current_term = term;
    
    for (int i = 1; i <= 50; i++) {
        result += current_term / (2 * i - 1);
        current_term *= term_sq;
        if (ABS(current_term) < EPSILON) break;
    }
    
    return 2.0 * result;
}

double compute_power(double base, int exponent) {
    if (exponent == 0) return 1.0;
    if (exponent < 0) {
        base = 1.0 / base;
        exponent = -exponent;
    }
    
    double result = 1.0;
    while (exponent > 0) {
        if (exponent % 2 == 1) {
            result *= base;
        }
        base *= base;
        exponent /= 2;
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
    printf("2. Natural logarithm (approximation)\n");
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
            result = compute_sqrt_newton(input_num);
            if (result < 0) {
                printf("Cannot compute square root of negative number.\n");
                return 1;
            }
            printf("Square root: %.10f\n", result);
            break;
            
        case 2:
            result = compute_log_approx(input_num);
            if (result < 0) {
                printf("Cannot compute logarithm of non-positive number.\n");
                return 1;
            }
            printf("Natural logarithm: %.10f\n", result);
            break;
            
        case 3:
            result = compute_power(input_num, 2);
            printf("Square: %.10f\n", result);
            break;
            
        case 4:
            result = compute_power(input_num, 3);
            printf("Cube: %.10f\n", result);
            break;
            
        default:
            printf("Invalid choice.\n");
            return 1;
    }
    
    return 0;
}