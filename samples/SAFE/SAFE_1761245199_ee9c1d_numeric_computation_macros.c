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
    if (exponent == 1) return base;
    
    double result = 1.0;
    int abs_exp = exponent < 0 ? -exponent : exponent;
    
    for (int i = 0; i < abs_exp; i++) {
        if (ABS(base) > 1.0 && result > DBL_MAX / ABS(base)) {
            return exponent > 0 ? DBL_MAX : 0.0;
        }
        if (ABS(base) < 1.0 && result < DBL_MIN / ABS(base)) {
            return exponent > 0 ? 0.0 : DBL_MAX;
        }
        result *= base;
    }
    
    return exponent < 0 ? 1.0 / result : result;
}

int main(void) {
    double number;
    int choice;
    
    printf("Enter a number between %.0f and %.0f: ", MIN_INPUT, MAX_INPUT);
    if (scanf("%lf", &number) != 1) {
        printf("Invalid input format.\n");
        return EXIT_FAILURE;
    }
    
    if (!VALIDATE_INPUT(number)) {
        printf("Input out of valid range.\n");
        return EXIT_FAILURE;
    }
    
    printf("Choose operation:\n");
    printf("1. Square root (Newton's method)\n");
    printf("2. Square\n");
    printf("3. Cube\n");
    printf("4. Power (integer exponent)\n");
    
    if (scanf("%d", &choice) != 1) {
        printf("Invalid choice format.\n");
        return EXIT_FAILURE;
    }
    
    switch (choice) {
        case 1: {
            double result = compute_sqrt_newton(number);
            if (result < 0) {
                printf("Cannot compute square root of negative number.\n");
                return EXIT_FAILURE;
            }
            printf("Square root: %.10f\n", result);
            break;
        }
        case 2: {
            double result = compute_power(number, 2);
            printf("Square: %.10f\n", result);
            break;
        }
        case 3: {
            double result = compute_power(number, 3);
            printf("Cube: %.10f\n", result);
            break;
        }
        case 4: {
            int exponent;
            printf("Enter integer exponent (-100 to 100): ");
            if (scanf("%d", &exponent) != 1) {
                printf("Invalid exponent format.\n");
                return EXIT_FAILURE;
            }
            if (exponent < -100 || exponent > 100) {
                printf("Exponent out of range.\n");
                return EXIT_FAILURE;
            }
            double result = compute_power(number, exponent);
            printf("Power: %.10f\n", result);
            break;
        }
        default:
            printf("Invalid choice.\n");
            return EXIT_FAILURE;
    }
    
    return EXIT_SUCCESS;
}