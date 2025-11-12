//DeepSeek-V3 V2.5 Category: Safe ; Style: modular_functions ; Variation: numeric_computation
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#define MAX_ITERATIONS 1000
#define EPSILON 1e-10

double compute_sqrt_newton(double x) {
    if (x < 0.0) {
        return -1.0;
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
        return -1.0;
    }
    double result = 1.0;
    for (int i = 0; i < exponent; i++) {
        if (result > 1e308 / base) {
            return -1.0;
        }
        result *= base;
    }
    return result;
}

double compute_factorial(int n) {
    if (n < 0) {
        return -1.0;
    }
    double result = 1.0;
    for (int i = 2; i <= n; i++) {
        if (result > 1e308 / i) {
            return -1.0;
        }
        result *= i;
    }
    return result;
}

int validate_integer_input(const char* input, int* value) {
    if (input == NULL || value == NULL) {
        return 0;
    }
    char* endptr;
    long temp = strtol(input, &endptr, 10);
    if (endptr == input || *endptr != '\0') {
        return 0;
    }
    if (temp < -1000000 || temp > 1000000) {
        return 0;
    }
    *value = (int)temp;
    return 1;
}

int validate_double_input(const char* input, double* value) {
    if (input == NULL || value == NULL) {
        return 0;
    }
    char* endptr;
    double temp = strtod(input, &endptr);
    if (endptr == input || *endptr != '\0') {
        return 0;
    }
    if (temp < -1e100 || temp > 1e100) {
        return 0;
    }
    *value = temp;
    return 1;
}

void display_menu(void) {
    printf("Numeric Computation Menu:\n");
    printf("1. Compute square root (Newton's method)\n");
    printf("2. Compute power (base^exponent)\n");
    printf("3. Compute factorial\n");
    printf("4. Exit\n");
    printf("Enter your choice (1-4): ");
}

int main(void) {
    char input_buffer[256];
    int choice;
    
    while (1) {
        display_menu();
        
        if (fgets(input_buffer, sizeof(input_buffer), stdin) == NULL) {
            printf("Input error.\n");
            continue;
        }
        
        if (!validate_integer_input(input_buffer, &choice)) {
            printf("Invalid choice. Please enter 1-4.\n");
            continue;
        }
        
        if (choice == 4) {
            printf("Goodbye!\n");
            break;
        }
        
        switch (choice) {
            case 1: {
                printf("Enter number for square root: ");
                if (fgets(input_buffer, sizeof(input_buffer), stdin) == NULL) {
                    printf("Input error.\n");
                    break;
                }
                double number;
                if (!validate_double_input(input_buffer, &number)) {
                    printf("Invalid number.\n");
                    break;
                }
                double result = compute_sqrt_newton(number);
                if (result < 0.0) {
                    printf("Cannot compute square root of negative number.\n");
                } else {
                    printf("Square root of %.6f is %.6f\n", number, result);
                }
                break;
            }
            
            case 2: {
                printf("Enter base: ");
                if (fgets(input_buffer, sizeof(input_buffer), stdin) == NULL) {
                    printf("Input error.\n");
                    break;
                }
                double base;
                if (!validate_double_input(input_buffer, &base)) {
                    printf("Invalid base.\n");
                    break;
                }
                
                printf("Enter exponent (non-negative integer): ");
                if (fgets(input_buffer, sizeof(input_buffer), stdin) == NULL) {
                    printf("Input error.\n");
                    break;
                }
                int exponent;
                if (!validate_integer_input(input_buffer, &exponent)) {
                    printf("Invalid exponent.\n");
                    break;
                }
                
                double result = compute_power(base, exponent);
                if (result < 0.0) {
                    printf("Power computation failed (overflow or invalid exponent).\n");
                } else {
                    printf("%.6f^%d = %.6f\n", base, exponent, result);
                }
                break;
            }
            
            case 3: {
                printf("Enter non-negative integer for factorial: ");
                if (fgets(input_buffer, sizeof(input_buffer), stdin) == NULL) {
                    printf("Input error.\n");
                    break;
                }
                int number;
                if (!validate_integer_input(input