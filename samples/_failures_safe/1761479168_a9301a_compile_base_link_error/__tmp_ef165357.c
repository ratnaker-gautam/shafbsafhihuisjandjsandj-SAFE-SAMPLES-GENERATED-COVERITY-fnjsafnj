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
    while (exponent > 0) {
        if (exponent % 2 == 1) {
            result *= base;
        }
        base *= base;
        exponent /= 2;
    }
    return result;
}

double compute_factorial(int n) {
    if (n < 0) return -1.0;
    if (n > 20) return -1.0;
    
    double result = 1.0;
    for (int i = 2; i <= n; i++) {
        result *= i;
    }
    return result;
}

int main() {
    double input1, input2;
    int choice, int_input;
    
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
            if (scanf("%lf", &input1) != 1) {
                printf("Invalid input format.\n");
                return 1;
            }
            if (!VALIDATE_INPUT(input1)) {
                printf("Input out of valid range.\n");
                return 1;
            }
            double sqrt_result = compute_sqrt_newton(input1);
            if (sqrt_result < 0) {
                printf("Cannot compute square root of negative number.\n");
            } else {
                printf("Square root: %.10f\n", sqrt_result);
                printf("Library sqrt: %.10f\n", sqrt(input1));
            }
            break;
            
        case 2:
            printf("Enter base: ");
            if (scanf("%lf", &input1) != 1) {
                printf("Invalid input format.\n");
                return 1;
            }
            printf("Enter integer exponent: ");
            if (scanf("%d", &int_input) != 1) {
                printf("Invalid input format.\n");
                return 1;
            }
            if (!VALIDATE_INPUT(input1)) {
                printf("Base out of valid range.\n");
                return 1;
            }
            if (int_input < -1000 || int_input > 1000) {
                printf("Exponent out of valid range.\n");
                return 1;
            }
            double power_result = compute_power(input1, int_input);
            if (power_result < 0 && input1 == 0.0 && int_input < 0) {
                printf("Cannot compute 0 to negative power.\n");
            } else {
                printf("Power result: %.10f\n", power_result);
            }
            break;
            
        case 3:
            printf("Enter integer for factorial: ");
            if (scanf("%d", &int_input) != 1) {
                printf("Invalid input format.\n");
                return 1;
            }
            double fact_result = compute_factorial(int_input);
            if (fact_result < 0) {
                if (int_input < 0) {
                    printf("Cannot compute factorial of negative number.\n");
                } else {
                    printf("Input too large for factorial calculation.\n");
                }
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