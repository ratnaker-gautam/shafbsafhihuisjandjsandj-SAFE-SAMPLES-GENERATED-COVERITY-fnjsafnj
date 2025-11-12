//DeepSeek-V3 V2.5 Category: Safe ; Style: modular_functions ; Variation: numeric_computation
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

void print_menu(void) {
    printf("Numeric Computation Menu:\n");
    printf("1. Compute square root (Newton's method)\n");
    printf("2. Compute power (base^exponent)\n");
    printf("3. Compute factorial\n");
    printf("4. Exit\n");
    printf("Enter your choice (1-4): ");
}

int get_valid_int(const char* prompt, int min_val, int max_val) {
    int value;
    char buffer[100];
    while (1) {
        printf("%s", prompt);
        if (fgets(buffer, sizeof(buffer), stdin) == NULL) {
            return -1;
        }
        if (sscanf(buffer, "%d", &value) == 1) {
            if (value >= min_val && value <= max_val) {
                return value;
            }
        }
        printf("Invalid input. Please enter an integer between %d and %d.\n", min_val, max_val);
    }
}

double get_valid_double(const char* prompt, double min_val, double max_val) {
    double value;
    char buffer[100];
    while (1) {
        printf("%s", prompt);
        if (fgets(buffer, sizeof(buffer), stdin) == NULL) {
            return NAN;
        }
        if (sscanf(buffer, "%lf", &value) == 1) {
            if (value >= min_val && value <= max_val) {
                return value;
            }
        }
        printf("Invalid input. Please enter a number between %.2f and %.2f.\n", min_val, max_val);
    }
}

int main(void) {
    int choice;
    double result;
    
    while (1) {
        print_menu();
        choice = get_valid_int("", 1, 4);
        
        if (choice == 4) {
            printf("Goodbye!\n");
            break;
        }
        
        switch (choice) {
            case 1: {
                double number = get_valid_double("Enter a non-negative number: ", 0.0, 1e308);
                result = compute_sqrt_newton(number);
                if (isnan(result)) {
                    printf("Error: Cannot compute square root of negative number.\n");
                } else {
                    printf("Square root of %.6f is %.10f\n", number, result);
                }
                break;
            }
            case 2: {
                double base = get_valid_double("Enter base (0.0 to 1000.0): ", 0.0, 1000.0);
                int exponent = get_valid_int("Enter non-negative exponent (0 to 100): ", 0, 100);
                result = compute_power(base, exponent);
                if (isnan(result)) {
                    printf("Error: Invalid exponent.\n");
                } else if (isinf(result)) {
                    printf("Error: Result too large.\n");
                } else {
                    printf("%.6f^%d = %.10f\n", base, exponent, result);
                }
                break;
            }
            case 3: {
                int n = get_valid_int("Enter non-negative integer (0 to 170): ", 0, 170);
                result = compute_factorial(n);
                if (isnan(result)) {
                    printf("Error: Cannot compute factorial of negative number.\n");
                } else if (isinf(result)) {
                    printf("Error: Factorial too large.\n");
                } else {
                    printf("%d! = %.0f\n", n, result);
                }
                break;
            }
        }
        printf("\n");
    }
    
    return 0;
}