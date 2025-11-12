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
    if (n < 0) return NAN;
    if (n == 0) return 0.0;
    
    double x = n;
    double prev;
    int iter = 0;
    
    do {
        prev = x;
        x = 0.5 * (x + n / x);
        iter++;
    } while (ABS(x - prev) > EPSILON && iter < MAX_ITERATIONS);
    
    return x;
}

double compute_power(double base, int exponent) {
    if (exponent == 0) return 1.0;
    if (exponent < 0) {
        base = 1.0 / base;
        exponent = -exponent;
    }
    
    double result = 1.0;
    while (exponent > 0) {
        if (exponent & 1) {
            result *= base;
        }
        base *= base;
        exponent >>= 1;
    }
    return result;
}

int main(void) {
    double input_num;
    int choice;
    
    printf("Enter a number between %.0f and %.0f: ", MIN_INPUT, MAX_INPUT);
    if (scanf("%lf", &input_num) != 1) {
        fprintf(stderr, "Error: Invalid input format\n");
        return EXIT_FAILURE;
    }
    
    if (!VALIDATE_INPUT(input_num)) {
        fprintf(stderr, "Error: Input out of valid range\n");
        return EXIT_FAILURE;
    }
    
    printf("Choose operation:\n");
    printf("1. Square root (Newton's method)\n");
    printf("2. Square\n");
    printf("3. Cube\n");
    printf("4. Natural logarithm (approximation)\n");
    
    if (scanf("%d", &choice) != 1) {
        fprintf(stderr, "Error: Invalid choice\n");
        return EXIT_FAILURE;
    }
    
    double result;
    switch (choice) {
        case 1:
            result = compute_sqrt_newton(input_num);
            if (isnan(result)) {
                fprintf(stderr, "Error: Cannot compute square root of negative number\n");
                return EXIT_FAILURE;
            }
            printf("Square root: %.10f\n", result);
            break;
            
        case 2:
            result = compute_power(input_num, 2);
            printf("Square: %.10f\n", result);
            break;
            
        case 3:
            result = compute_power(input_num, 3);
            printf("Cube: %.10f\n", result);
            break;
            
        case 4:
            if (input_num <= 0) {
                fprintf(stderr, "Error: Logarithm undefined for non-positive numbers\n");
                return EXIT_FAILURE;
            }
            result = log(input_num);
            printf("Natural logarithm: %.10f\n", result);
            break;
            
        default:
            fprintf(stderr, "Error: Invalid operation choice\n");
            return EXIT_FAILURE;
    }
    
    return EXIT_SUCCESS;
}