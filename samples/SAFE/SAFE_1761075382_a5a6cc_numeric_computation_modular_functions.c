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

int read_integer(const char* prompt, int min, int max) {
    int value;
    char buffer[100];
    while (1) {
        printf("%s", prompt);
        if (fgets(buffer, sizeof(buffer), stdin) == NULL) {
            return -1;
        }
        if (sscanf(buffer, "%d", &value) == 1) {
            if (value >= min && value <= max) {
                return value;
            }
        }
        printf("Invalid input. Please enter an integer between %d and %d.\n", min, max);
    }
}

double read_double(const char* prompt, double min, double max) {
    double value;
    char buffer[100];
    while (1) {
        printf("%s", prompt);
        if (fgets(buffer, sizeof(buffer), stdin) == NULL) {
            return NAN;
        }
        if (sscanf(buffer, "%lf", &value) == 1) {
            if (value >= min && value <= max) {
                return value;
            }
        }
        printf("Invalid input. Please enter a number between %.2f and %.2f.\n", min, max);
    }
}

void display_menu() {
    printf("\nNumeric Computation Menu:\n");
    printf("1. Compute square root (Newton's method)\n");
    printf("2. Compute power (integer exponent)\n");
    printf("3. Compute factorial\n");
    printf("4. Exit\n");
}

int main() {
    int choice;
    double input_num, result;
    int input_int;

    printf("Numeric Computation Program\n");
    printf("===========================\n");

    while (1) {
        display_menu();
        choice = read_integer("Enter your choice (1-4): ", 1, 4);
        
        if (choice == 1) {
            input_num = read_double("Enter a non-negative number: ", 0.0, 1e308);
            result = compute_sqrt_newton(input_num);
            if (isnan(result)) {
                printf("Error: Cannot compute square root of negative number.\n");
            } else {
                printf("Square root of %.6f is %.10f\n", input_num, result);
            }
        } else if (choice == 2) {
            input_num = read_double("Enter base: ", -1e308, 1e308);
            input_int = read_integer("Enter non-negative exponent: ", 0, 10000);
            result = compute_power(input_num, input_int);
            if (isnan(result)) {
                printf("Error: Invalid exponent.\n");
            } else if (isinf(result)) {
                printf("Error: Result too large.\n");
            } else {
                printf("%.6f ^ %d = %.10f\n", input_num, input_int, result);
            }
        } else if (choice == 3) {
            input_int = read_integer("Enter non-negative integer (0-170): ", 0, 170);
            result = compute_factorial(input_int);
            if (isinf(result)) {
                printf("Error: Factorial too large.\n");
            } else {
                printf("%d! = %.0f\n", input_int, result);
            }
        } else if (choice == 4) {
            printf("Goodbye!\n");
            break;
        }
    }

    return 0;
}