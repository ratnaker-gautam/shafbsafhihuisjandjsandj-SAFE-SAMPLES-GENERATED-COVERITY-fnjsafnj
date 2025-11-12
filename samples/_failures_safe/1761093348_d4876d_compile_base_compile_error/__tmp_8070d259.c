//DeepSeek-V3 SAFE v1.4 Category: Safe ; Style: modular_functions ; Variation: numeric_computation
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <limits.h>

#define MAX_ITERATIONS 1000
#define EPSILON 1e-10

double compute_sqrt(double x) {
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
    return 1;
}

void display_menu() {
    printf("Numeric Computation Menu:\n");
    printf("1. Compute square root\n");
    printf("2. Compute power\n");
    printf("3. Compute factorial\n");
    printf("4. Exit\n");
    printf("Enter your choice (1-4): ");
}

int main() {
    char input_buffer[100];
    int choice;
    double num1, num2;
    int int_val;
    
    while (1) {
        display_menu();
        
        if (fgets(input_buffer, sizeof(input_buffer), stdin) == NULL) {
            break;
        }
        
        if (!validate_integer_input(input_buffer, &choice)) {
            printf("Invalid choice. Please enter a number between 1 and 4.\n\n");
            continue;
        }
        
        switch (choice) {
            case 1:
                printf("Enter a non-negative number: ");
                if (fgets(input_buffer, sizeof(input_buffer), stdin) == NULL) {
                    break;
                }
                if (!validate_double_input(input_buffer, &num1)) {
                    printf("Invalid number format.\n\n");
                    break;
                }
                if (num1 < 0) {
                    printf("Cannot compute square root of negative number.\n\n");
                    break;
                }
                printf("Square root of %.6f is %.6f\n\n", num1, compute_sqrt(num1));
                break;
                
            case 2:
                printf("Enter base: ");
                if (fgets(input_buffer, sizeof(input_buffer), stdin) == NULL) {
                    break;
                }
                if (!validate_double_input(input_buffer, &num1)) {
                    printf("Invalid base format.\n\n");
                    break;
                }
                printf("Enter non-negative integer exponent: ");
                if (fgets(input_buffer, sizeof(input_buffer), stdin) == NULL) {
                    break;
                }
                if (!validate_integer_input(input_buffer, &int_val)) {
                    printf("Invalid exponent format.\n\n");
                    break;
                }
                if (int_val < 0) {
                    printf("Exponent must be non-negative.\n\n");
                    break;
                }
                double power_result = compute_power(num1, int_val);
                if (power_result < 0) {
                    printf("Result too large or invalid.\n\n");
                } else {
                    printf("%.6f^%d = %.6f\n\n", num1, int_val, power_result);
                }
                break;
                
            case 3:
                printf("Enter non-negative integer: ");
                if (fgets(input_buffer, sizeof(input_buffer), stdin) == NULL) {
                    break;
                }
                if (!validate_integer_input(input_buffer, &int_val)) {
                    printf("Invalid integer format.\n\n");
                    break;
                }
                if (int_val < 0) {
                    printf("Cannot compute factorial of negative number.\n\n");
                    break;
                }
                double fact_result = compute_factorial(int_val);
                if (fact_result < 0) {
                    printf("Factorial too large to compute.\n\n");
                } else {
                    printf("%d! = %.0f\n\n", int_val, fact_result);
                }
                break;
                
            case 4:
                printf