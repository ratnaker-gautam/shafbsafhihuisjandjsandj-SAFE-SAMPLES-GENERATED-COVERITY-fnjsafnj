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
    }
    return result;
}

double compute_factorial(int n) {
    if (n < 0) {
        return NAN;
    }
    double result = 1.0;
    for (int i = 2; i <= n; i++) {
        result *= i;
    }
    return result;
}

int validate_integer_input(const char* input, int* value) {
    char* endptr;
    long temp = strtol(input, &endptr, 10);
    if (*endptr != '\0' || *endptr == '\n' || temp < INT_MIN || temp > INT_MAX) {
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
    return 1;
}

void display_menu(void) {
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
    
    while (1) {
        display_menu();
        
        if (fgets(input_buffer, sizeof(input_buffer), stdin) == NULL) {
            break;
        }
        
        if (!validate_integer_input(input_buffer, &choice)) {
            printf("Invalid choice. Please enter a number between 1 and 4.\n");
            continue;
        }
        
        if (choice == 4) {
            printf("Exiting program.\n");
            break;
        }
        
        if (choice < 1 || choice > 3) {
            printf("Invalid choice. Please enter a number between 1 and 4.\n");
            continue;
        }
        
        switch (choice) {
            case 1: {
                double number;
                printf("Enter a non-negative number: ");
                if (fgets(input_buffer, sizeof(input_buffer), stdin) == NULL) {
                    printf("Input error.\n");
                    break;
                }
                if (!validate_double_input(input_buffer, &number)) {
                    printf("Invalid number format.\n");
                    break;
                }
                double result = compute_sqrt_newton(number);
                if (isnan(result)) {
                    printf("Cannot compute square root of negative number.\n");
                } else {
                    printf("Square root of %.6f is %.6f\n", number, result);
                }
                break;
            }
            
            case 2: {
                double base;
                int exponent;
                printf("Enter base: ");
                if (fgets(input_buffer, sizeof(input_buffer), stdin) == NULL) {
                    printf("Input error.\n");
                    break;
                }
                if (!validate_double_input(input_buffer, &base)) {
                    printf("Invalid base format.\n");
                    break;
                }
                printf("Enter non-negative integer exponent: ");
                if (fgets(input_buffer, sizeof(input_buffer), stdin) == NULL) {
                    printf("Input error.\n");
                    break;
                }
                if (!validate_integer_input(input_buffer, &exponent)) {
                    printf("Invalid exponent format.\n");
                    break;
                }
                if (exponent < 0) {
                    printf("Exponent must be non-negative.\n");
                    break;
                }
                double result = compute_power(base, exponent);
                printf("%.6f^%d = %.6f\n", base, exponent, result);
                break;
            }
            
            case 3: {
                int n;
                printf("Enter non-negative integer: ");
                if (fgets(input_buffer, sizeof(input_buffer), stdin) == NULL) {
                    printf("Input error.\n");
                    break;
                }
                if (!validate_integer_input(input_buffer, &n)) {
                    printf("Invalid integer format.\n");
                    break;
                }
                if (n < 0) {
                    printf("Input must be non-negative.\n");
                    break;
                }
                double result = compute_factorial(n);
                printf("%d! = %.0f\n", n, result);
                break;
            }
        }
    }
    
    return 0;
}