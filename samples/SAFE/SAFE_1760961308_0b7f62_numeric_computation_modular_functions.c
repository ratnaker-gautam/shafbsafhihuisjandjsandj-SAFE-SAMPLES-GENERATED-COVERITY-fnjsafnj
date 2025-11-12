//DeepSeek-V3 SAFE v1.4 Category: Safe ; Style: modular_functions ; Variation: numeric_computation
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#define MAX_ITERATIONS 1000
#define EPSILON 1e-10

double compute_sqrt_newton(double x) {
    if (x < 0.0) {
        return NAN;
    }
    if (x == 0.0) {
        return 0.0;
    }
    double guess = x / 2.0;
    for (int i = 0; i < MAX_ITERATIONS; i++) {
        double new_guess = 0.5 * (guess + x / guess);
        if (fabs(new_guess - guess) < EPSILON) {
            return new_guess;
        }
        guess = new_guess;
    }
    return guess;
}

double compute_power(double base, int exponent) {
    if (exponent < 0) {
        return NAN;
    }
    double result = 1.0;
    for (int i = 0; i < exponent; i++) {
        if (result > 1e308 / base) {
            return INFINITY;
        }
        result *= base;
    }
    return result;
}

double compute_factorial(int n) {
    if (n < 0) {
        return NAN;
    }
    if (n > 170) {
        return INFINITY;
    }
    double result = 1.0;
    for (int i = 2; i <= n; i++) {
        result *= i;
    }
    return result;
}

void print_result(double result) {
    if (isnan(result)) {
        printf("Result: undefined\n");
    } else if (isinf(result)) {
        printf("Result: overflow\n");
    } else {
        printf("Result: %.10g\n", result);
    }
}

int main() {
    int choice;
    double input1, input2;
    int input_int;
    double result;
    
    printf("Numeric Computation Menu:\n");
    printf("1. Square root (Newton's method)\n");
    printf("2. Power (base^exponent)\n");
    printf("3. Factorial\n");
    printf("Enter choice (1-3): ");
    
    if (scanf("%d", &choice) != 1) {
        printf("Invalid input\n");
        return 1;
    }
    
    switch (choice) {
        case 1:
            printf("Enter number: ");
            if (scanf("%lf", &input1) != 1) {
                printf("Invalid input\n");
                return 1;
            }
            result = compute_sqrt_newton(input1);
            print_result(result);
            break;
            
        case 2:
            printf("Enter base: ");
            if (scanf("%lf", &input1) != 1) {
                printf("Invalid input\n");
                return 1;
            }
            printf("Enter exponent (non-negative integer): ");
            if (scanf("%d", &input_int) != 1) {
                printf("Invalid input\n");
                return 1;
            }
            result = compute_power(input1, input_int);
            print_result(result);
            break;
            
        case 3:
            printf("Enter integer (0-170): ");
            if (scanf("%d", &input_int) != 1) {
                printf("Invalid input\n");
                return 1;
            }
            result = compute_factorial(input_int);
            print_result(result);
            break;
            
        default:
            printf("Invalid choice\n");
            return 1;
    }
    
    return 0;
}