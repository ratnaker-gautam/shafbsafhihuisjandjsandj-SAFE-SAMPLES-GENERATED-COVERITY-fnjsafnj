//DeepSeek-V3 V2.5 Category: Safe ; Style: modular_functions ; Variation: numeric_computation
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <limits.h>

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
        result *= base;
        if (isinf(result) || isnan(result)) {
            return NAN;
        }
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
        if (isinf(result)) {
            return INFINITY;
        }
    }
    return result;
}

int validate_integer_input(const char* input, int* value) {
    char* endptr;
    long temp = strtol(input, &endptr, 10);
    if (*endptr != '\0' && *endptr != '\n') {
        return 0;
    }
    if (temp < INT_MIN || temp > INT_MAX) {
        return 0;
    }
    *value = (int)temp;
    return 1;
}

int validate_double_input(const char* input, double* value) {
    char* endptr;
    *value = strtod(input, &endptr);
    if (*endptr != '\0' && *endptr != '\n') {
        return 0;
    }
    if (isinf(*value) || isnan(*value)) {
        return 0;
    }
    return 1;
}

void print_menu(void) {
    printf("Numeric Computation Menu:\n");
    printf("1. Compute square root (Newton's method)\n");
    printf("2. Compute power (integer exponent)\n");
    printf("3. Compute factorial\n");
    printf("4. Exit\n");
    printf("Enter your choice (1-4): ");
}

int main(void) {
    char input_buffer[256];
    int choice;
    double num1, result;
    int num2;
    
    while (1) {
        print_menu();
        
        if (fgets(input_buffer, sizeof(input_buffer), stdin) == NULL) {
            break;
        }
        
        if (!validate_integer_input(input_buffer, &choice)) {
            printf("Invalid choice. Please enter 1-4.\n\n");
            continue;
        }
        
        switch (choice) {
            case 1:
                printf("Enter number for square root: ");
                if (fgets(input_buffer, sizeof(input_buffer), stdin) == NULL) {
                    printf("Input error.\n\n");
                    continue;
                }
                if (!validate_double_input(input_buffer, &num1)) {
                    printf("Invalid number.\n\n");
                    continue;
                }
                result = compute_sqrt_newton(num1);
                if (isnan(result)) {
                    printf("Cannot compute square root of negative number.\n\n");
                } else {
                    printf("Square root: %.10f\n\n", result);
                }
                break;
                
            case 2:
                printf("Enter base: ");
                if (fgets(input_buffer, sizeof(input_buffer), stdin) == NULL) {
                    printf("Input error.\n\n");
                    continue;
                }
                if (!validate_double_input(input_buffer, &num1)) {
                    printf("Invalid base.\n\n");
                    continue;
                }
                printf("Enter exponent (non-negative integer): ");
                if (fgets(input_buffer, sizeof(input_buffer), stdin) == NULL) {
                    printf("Input error.\n\n");
                    continue;
                }
                if (!validate_integer_input(input_buffer, &num2) || num2 < 0) {
                    printf("Invalid exponent.\n\n");
                    continue;
                }
                result = compute_power(num1, num2);
                if (isnan(result)) {
                    printf("Result is too large or undefined.\n\n");
                } else {
                    printf("Power result: %.10f\n\n", result);
                }
                break;
                
            case 3:
                printf("Enter integer (0-170): ");
                if (fgets(input_buffer, sizeof(input_buffer), stdin) == NULL) {
                    printf("Input error.\n\n");
                    continue;
                }
                if (!validate_integer_input(input_buffer, &num2) || num2 < 0 || num2 > 170) {
                    printf("Invalid input. Must be integer 0-170.\n\n");
                    continue;
                }
                result = compute_factorial(num2);
                if (isinf(result)) {
                    printf("Factorial too large to represent exactly.\n\n");
                } else {
                    printf("Factorial: %.0f