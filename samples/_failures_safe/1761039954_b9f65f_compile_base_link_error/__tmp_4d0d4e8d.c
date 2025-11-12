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
    
    double guess = n / 2.0;
    for (int i = 0; i < MAX_ITERATIONS; i++) {
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
    if (exponent < 0) {
        if (base == 0.0) return -1.0;
        base = 1.0 / base;
        exponent = -exponent;
    }
    
    double result = 1.0;
    for (int i = 0; i < exponent; i++) {
        if (result > MAX_INPUT / ABS(base)) return -1.0;
        result *= base;
    }
    return result;
}

double compute_factorial(int n) {
    if (n < 0) return -1.0;
    if (n == 0) return 1.0;
    
    double result = 1.0;
    for (int i = 1; i <= n; i++) {
        if (result > MAX_INPUT / i) return -1.0;
        result *= i;
    }
    return result;
}

int main(void) {
    double input_num;
    int input_int;
    int choice;
    
    printf("Numeric Computation Menu:\n");
    printf("1. Square Root (Newton's Method)\n");
    printf("2. Power Calculation\n");
    printf("3. Factorial\n");
    printf("Enter choice (1-3): ");
    
    if (scanf("%d", &choice) != 1) {
        printf("Invalid input format.\n");
        return 1;
    }
    
    switch (choice) {
        case 1:
            printf("Enter number for square root: ");
            if (scanf("%lf", &input_num) != 1) {
                printf("Invalid input format.\n");
                return 1;
            }
            if (!VALIDATE_INPUT(input_num)) {
                printf("Input out of valid range.\n");
                return 1;
            }
            double sqrt_result = compute_sqrt_newton(input_num);
            if (sqrt_result < 0) {
                printf("Cannot compute square root of negative number.\n");
            } else {
                printf("Square root: %.10f\n", sqrt_result);
                printf("Library sqrt: %.10f\n", sqrt(input_num));
            }
            break;
            
        case 2:
            printf("Enter base: ");
            if (scanf("%lf", &input_num) != 1) {
                printf("Invalid input format.\n");
                return 1;
            }
            if (!VALIDATE_INPUT(input_num)) {
                printf("Input out of valid range.\n");
                return 1;
            }
            printf("Enter integer exponent: ");
            if (scanf("%d", &input_int) != 1) {
                printf("Invalid input format.\n");
                return 1;
            }
            if (ABS(input_int) > 1000) {
                printf("Exponent too large.\n");
                return 1;
            }
            double power_result = compute_power(input_num, input_int);
            if (power_result < 0) {
                printf("Power computation failed (overflow or division by zero).\n");
            } else {
                printf("Power result: %.10f\n", power_result);
            }
            break;
            
        case 3:
            printf("Enter integer for factorial: ");
            if (scanf("%d", &input_int) != 1) {
                printf("Invalid input format.\n");
                return 1;
            }
            if (input_int < 0 || input_int > 100) {
                printf("Input must be between 0 and 100.\n");
                return 1;
            }
            double fact_result = compute_factorial(input_int);
            if (fact_result < 0) {
                printf("Factorial computation overflow.\n");
            } else {
                printf("Factorial: %.0f\n", fact_result);
            }
            break;
            
        default:
            printf("Invalid choice.\n");
            return 1;
    }
    
    return 0;
}