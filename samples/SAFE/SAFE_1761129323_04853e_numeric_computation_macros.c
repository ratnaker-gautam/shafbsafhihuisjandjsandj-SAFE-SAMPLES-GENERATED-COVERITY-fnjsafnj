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
        if (fabs(guess) < EPSILON) {
            guess = EPSILON;
        }
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
    if (exponent == 1) return base;
    
    int abs_exp = exponent < 0 ? -exponent : exponent;
    double result = 1.0;
    
    for (int i = 0; i < abs_exp; i++) {
        if (result > DBL_MAX / base) {
            return exponent > 0 ? DBL_MAX : 0.0;
        }
        result *= base;
    }
    
    return exponent < 0 ? 1.0 / result : result;
}

int main() {
    double input_num;
    int choice;
    
    printf("Enter a number between %.0f and %.0f: ", MIN_INPUT, MAX_INPUT);
    if (scanf("%lf", &input_num) != 1) {
        printf("Invalid input format.\n");
        return EXIT_FAILURE;
    }
    
    if (!VALIDATE_INPUT(input_num)) {
        printf("Input out of valid range.\n");
        return EXIT_FAILURE;
    }
    
    printf("Choose operation:\n");
    printf("1. Square root (Newton's method)\n");
    printf("2. Square\n");
    printf("3. Cube\n");
    printf("4. Reciprocal\n");
    
    if (scanf("%d", &choice) != 1) {
        printf("Invalid choice format.\n");
        return EXIT_FAILURE;
    }
    
    double result;
    
    switch (choice) {
        case 1:
            result = compute_sqrt_newton(input_num);
            if (result < 0) {
                printf("Cannot compute square root of negative number.\n");
                return EXIT_FAILURE;
            }
            printf("Square root: %.10f\n", result);
            break;
            
        case 2:
            result = compute_power(input_num, 2);
            if (result == DBL_MAX) {
                printf("Result too large.\n");
                return EXIT_FAILURE;
            }
            printf("Square: %.10f\n", result);
            break;
            
        case 3:
            result = compute_power(input_num, 3);
            if (result == DBL_MAX) {
                printf("Result too large.\n");
                return EXIT_FAILURE;
            }
            printf("Cube: %.10f\n", result);
            break;
            
        case 4:
            if (fabs(input_num) < EPSILON) {
                printf("Cannot compute reciprocal of zero.\n");
                return EXIT_FAILURE;
            }
            result = 1.0 / input_num;
            printf("Reciprocal: %.10f\n", result);
            break;
            
        default:
            printf("Invalid choice.\n");
            return EXIT_FAILURE;
    }
    
    return EXIT_SUCCESS;
}