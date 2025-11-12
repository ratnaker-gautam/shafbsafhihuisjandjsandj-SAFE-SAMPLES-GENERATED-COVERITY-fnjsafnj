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
    if (*endptr != '\0' || temp < INT_MIN || temp > INT_MAX) {
        return 0;
    }
    *value = (int)temp;
    return 1;
}

int validate_double_input(const char* input, double* value) {
    char* endptr;
    *value = strtod(input, &endptr);
    if (*endptr != '\0') {
        return 0;
    }
    return 1;
}

void display_menu(void) {
    printf("Numeric Computation Menu:\n");
    printf("1. Compute square root\n");
    printf("2. Compute power\n");
    printf("3. Compute factorial\n");
    printf("4. Exit\n");
    printf("Enter your choice (1-4): ");
}

int main(void) {
    char input_buffer[100];
    int choice;
    double num1, num2, result;
    int int_val;

    while (1) {
        display_menu();
        
        if (fgets(input_buffer, sizeof(input_buffer), stdin) == NULL) {
            break;
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
            case 1:
                printf("Enter a non-negative number: ");
                if (fgets(input_buffer, sizeof(input_buffer), stdin) == NULL) break;
                if (!validate_double_input(input_buffer, &num1)) {
                    printf("Invalid number.\n");
                    break;
                }
                result = compute_sqrt(num1);
                if (result < 0) {
                    printf("Cannot compute square root of negative number.\n");
                } else {
                    printf("Square root: %.10f\n", result);
                }
                break;

            case 2:
                printf("Enter base: ");
                if (fgets(input_buffer, sizeof(input_buffer), stdin) == NULL) break;
                if (!validate_double_input(input_buffer, &num1)) {
                    printf("Invalid base.\n");
                    break;
                }
                printf("Enter non-negative integer exponent: ");
                if (fgets(input_buffer, sizeof(input_buffer), stdin) == NULL) break;
                if (!validate_integer_input(input_buffer, &int_val) || int_val < 0) {
                    printf("Invalid exponent.\n");
                    break;
                }
                result = compute_power(num1, int_val);
                if (result < 0) {
                    printf("Power computation overflow.\n");
                } else {
                    printf("Power result: %.10f\n", result);
                }
                break;

            case 3:
                printf("Enter non-negative integer: ");
                if (fgets(input_buffer, sizeof(input_buffer), stdin) == NULL) break;
                if (!validate_integer_input(input_buffer, &int_val) || int_val < 0) {
                    printf("Invalid input.\n");
                    break;
                }
                result = compute_factorial(int_val);
                if (result < 0) {
                    printf("Factorial overflow.\n");
                } else {
                    printf("Factorial: %.0f\n", result);
                }
                break;

            default:
                printf("Invalid choice. Please enter 1-4.\n");
                break;
        }
    }

    return 0;
}