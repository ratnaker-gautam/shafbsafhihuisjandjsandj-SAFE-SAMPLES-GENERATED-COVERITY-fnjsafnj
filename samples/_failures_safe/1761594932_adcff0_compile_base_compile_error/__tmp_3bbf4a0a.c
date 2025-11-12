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
    if (terms < 1) {
        return NAN;
    }
    double sum = 0.0;
    for (int i = 1; i <= terms; i++) {
        sum += 1.0 / compute_power(2.0, i);
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

void display_menu() {
    printf("\nNumeric Computation Menu:\n");
    printf("1. Compute square root (Newton's method)\n");
    printf("2. Compute power (integer exponent)\n");
    printf("3. Compute factorial\n");
    printf("4. Compute series sum (1/2 + 1/4 + 1/8 + ...)\n");
    printf("5. Exit\n");
    printf("Enter your choice (1-5): ");
}

int main() {
    printf("Numeric Computation Program\n");
    
    while (1) {
        display_menu();
        
        int choice = get_valid_integer("", 1, 5);
        if (choice == -1) {
            break;
        }
        
        switch (choice) {
            case 1: {
                double num = get_valid_double("Enter a non-negative number: ", 0.0, 1e9);
                if (isnan(num)) break;
                double result = compute_sqrt_newton(num);
                if (isnan(result)) {
                    printf("Cannot compute square root of negative number.\n");
                } else {
                    printf("Square root of %.6f is %.10f\n", num, result);
                }
                break;
            }
            case 2: {
                double base = get_valid_double("Enter base: ", -100.0, 100.0);
                if (isnan(base)) break;
                int exp = get_valid_integer("Enter non-negative exponent: ", 0, 100);
                if (exp == -1) break;
                double result = compute_power(base, exp);
                if (isnan(result)) {
                    printf("Invalid exponent.\n");
                } else {
                    printf("%.6f^%d = %.10f\n", base, exp, result);
                }
                break;
            }
            case 3: {
                int n = get_valid_integer("Enter non-negative integer: ", 0, 170);
                if (n == -1) break;
                double result = compute_factorial(n);
                if (isnan(result)) {
                    printf("Cannot compute factorial of negative number.\n");
                } else {
                    printf("%d! = %.0f\n", n, result);
                }
                break;
            }
            case 4: {
                int terms = get_valid_integer("Enter number of terms (1-1000): ", 1, 1000);
                if (terms == -1) break;
                double result = compute_series_sum(terms);
                printf("Sum of first %d terms: %.10f\n", terms, result);