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
    if (exponent == 0) {
        return 1.0;
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
    if (n == 0 || n == 1) {
        return 1.0;
    }
    double result = 1.0;
    for (int i = 2; i <= n; i++) {
        if (result > 1e308 / i) {
            return INFINITY;
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
    double base, result;
    int exponent, n;

    while (1) {
        display_menu();
        if (fgets(input_buffer, sizeof(input_buffer), stdin) == NULL) {
            break;
        }
        if (!validate_integer_input(input_buffer, &choice)) {
            printf("Invalid choice. Please enter a number between 1 and 4.\n");
            continue;
        }

        switch (choice) {
            case 1:
                printf("Enter a non-negative number: ");
                if (fgets(input_buffer, sizeof(input_buffer), stdin) == NULL) {
                    break;
                }
                if (!validate_double_input(input_buffer, &base)) {
                    printf("Invalid number format.\n");
                    break;
                }
                result = compute_sqrt_newton(base);
                if (isnan(result)) {
                    printf("Cannot compute square root of negative number.\n");
                } else {
                    printf("Square root of %.6f is %.6f\n", base, result);
                }
                break;

            case 2:
                printf("Enter base: ");
                if (fgets(input_buffer, sizeof(input_buffer), stdin) == NULL) {
                    break;
                }
                if (!validate_double_input(input_buffer, &base)) {
                    printf("Invalid base format.\n");
                    break;
                }
                printf("Enter non-negative integer exponent: ");
                if (fgets(input_buffer, sizeof(input_buffer), stdin) == NULL) {
                    break;
                }
                if (!validate_integer_input(input_buffer, &exponent) || exponent < 0) {
                    printf("Invalid exponent.\n");
                    break;
                }
                result = compute_power(base, exponent);
                if (isnan(result)) {
                    printf("Invalid input for power computation.\n");
                } else if (isinf(result)) {
                    printf("Result exceeds representable range.\n");
                } else {
                    printf("%.6f^%d = %.6f\n", base, exponent, result);
                }
                break;

            case 3:
                printf("Enter non-negative integer: ");
                if (fgets(input_buffer, sizeof(input_buffer), stdin) == NULL) {
                    break;
                }
                if (!validate_integer_input(input_buffer, &n) || n < 0) {
                    printf("Invalid input. Must be non-negative integer.\n");
                    break;
                }
                result = compute_factorial(n);
                if (isnan(result)) {
                    printf("Invalid input for factorial.\n");
                } else if (isinf(result)) {
                    printf("Factorial exceeds representable range.\n");
                } else {
                    printf("%d! = %.0f\n",