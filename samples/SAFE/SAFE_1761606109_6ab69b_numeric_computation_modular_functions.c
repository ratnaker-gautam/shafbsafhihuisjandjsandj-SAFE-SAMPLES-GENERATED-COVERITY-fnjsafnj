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

double compute_series_sum(int terms) {
    if (terms <= 0) {
        return NAN;
    }
    double sum = 0.0;
    for (int i = 1; i <= terms; i++) {
        double term = 1.0 / compute_power(i, 2);
        sum += term;
    }
    return sum;
}

int get_valid_integer(const char* prompt, int min_val, int max_val) {
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

int main() {
    printf("Numeric Computation Tool\n");
    printf("1. Compute square root (Newton's method)\n");
    printf("2. Compute power (integer exponent)\n");
    printf("3. Compute factorial\n");
    printf("4. Compute series sum (1/n^2)\n");
    
    int choice = get_valid_integer("Enter choice (1-4): ", 1, 4);
    
    switch (choice) {
        case 1: {
            double num = get_valid_double("Enter number (0-1000): ", 0.0, 1000.0);
            double result = compute_sqrt_newton(num);
            if (isnan(result)) {
                printf("Error: Cannot compute square root of negative number.\n");
            } else {
                printf("Square root of %.2f: %.10f\n", num, result);
            }
            break;
        }
        case 2: {
            double base = get_valid_double("Enter base (-100-100): ", -100.0, 100.0);
            int exponent = get_valid_integer("Enter exponent (0-20): ", 0, 20);
            double result = compute_power(base, exponent);
            if (isnan(result)) {
                printf("Error: Negative exponents not supported.\n");
            } else {
                printf("%.2f^%d = %.10f\n", base, exponent, result);
            }
            break;
        }
        case 3: {
            int n = get_valid_integer("Enter integer (0-20): ", 0, 20);
            double result = compute_factorial(n);
            if (isnan(result)) {
                printf("Error: Cannot compute factorial of negative number.\n");
            } else {
                printf("%d! = %.0f\n", n, result);
            }
            break;
        }
        case 4: {
            int terms = get_valid_integer("Enter number of terms (1-1000): ", 1, 1000);
            double result = compute_series_sum(terms);
            printf("Sum of first %d terms of 1/n^2: %.10f\n", terms, result);
            break;
        }
    }
    
    return 0;
}