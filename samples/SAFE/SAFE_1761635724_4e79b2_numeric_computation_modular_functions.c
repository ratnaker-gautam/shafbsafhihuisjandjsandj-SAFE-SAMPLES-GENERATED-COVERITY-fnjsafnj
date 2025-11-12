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
        double term = 1.0 / compute_power(i, 2);
        if (isnan(term)) {
            return NAN;
        }
        sum += term;
    }
    return sum;
}

int get_valid_integer(const char* prompt, int min_val, int max_val) {
    printf("%s", prompt);
    int value;
    if (scanf("%d", &value) != 1) {
        return -1;
    }
    if (value < min_val || value > max_val) {
        return -1;
    }
    return value;
}

double get_valid_double(const char* prompt, double min_val, double max_val) {
    printf("%s", prompt);
    double value;
    if (scanf("%lf", &value) != 1) {
        return NAN;
    }
    if (value < min_val || value > max_val) {
        return NAN;
    }
    return value;
}

int main() {
    printf("Numeric Computation Demo\n");
    printf("1. Compute square root (Newton's method)\n");
    printf("2. Compute power (integer exponent)\n");
    printf("3. Compute factorial\n");
    printf("4. Compute series sum (1/n^2)\n");
    printf("Enter choice (1-4): ");
    
    int choice;
    if (scanf("%d", &choice) != 1) {
        printf("Invalid input\n");
        return 1;
    }
    
    double result;
    
    switch (choice) {
        case 1: {
            double num = get_valid_double("Enter number (0-1000): ", 0.0, 1000.0);
            if (isnan(num)) {
                printf("Invalid input\n");
                return 1;
            }
            result = compute_sqrt_newton(num);
            if (isnan(result)) {
                printf("Cannot compute square root of negative number\n");
                return 1;
            }
            printf("Square root: %.6f\n", result);
            break;
        }
        case 2: {
            double base = get_valid_double("Enter base (-100-100): ", -100.0, 100.0);
            if (isnan(base)) {
                printf("Invalid input\n");
                return 1;
            }
            int exp = get_valid_integer("Enter exponent (0-20): ", 0, 20);
            if (exp == -1) {
                printf("Invalid input\n");
                return 1;
            }
            result = compute_power(base, exp);
            if (isnan(result)) {
                printf("Invalid exponent\n");
                return 1;
            }
            printf("Power: %.6f\n", result);
            break;
        }
        case 3: {
            int n = get_valid_integer("Enter integer (0-20): ", 0, 20);
            if (n == -1) {
                printf("Invalid input\n");
                return 1;
            }
            result = compute_factorial(n);
            if (isnan(result)) {
                printf("Invalid input\n");
                return 1;
            }
            printf("Factorial: %.0f\n", result);
            break;
        }
        case 4: {
            int terms = get_valid_integer("Enter number of terms (1-1000): ", 1, 1000);
            if (terms == -1) {
                printf("Invalid input\n");
                return 1;
            }
            result = compute_series_sum(terms);
            if (isnan(result)) {
                printf("Computation error\n");
                return 1;
            }
            printf("Series sum: %.10f\n", result);
            break;
        }
        default:
            printf("Invalid choice\n");
            return 1;
    }
    
    return 0;
}