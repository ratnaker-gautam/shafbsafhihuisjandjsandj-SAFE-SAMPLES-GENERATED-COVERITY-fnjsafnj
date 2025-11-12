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
        if (isnan(term)) {
            return NAN;
        }
        sum += term;
    }
    return sum;
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

int main() {
    printf("Numeric Computation Demo\n");
    printf("=======================\n");
    
    double number = get_valid_double("Enter a positive number for square root calculation (0-1000): ", 0.0, 1000.0);
    if (isnan(number)) {
        printf("Input error occurred.\n");
        return 1;
    }
    
    int exponent = get_valid_int("Enter a non-negative exponent for power calculation (0-20): ", 0, 20);
    if (exponent == -1) {
        printf("Input error occurred.\n");
        return 1;
    }
    
    int factorial_num = get_valid_int("Enter a non-negative integer for factorial (0-20): ", 0, 20);
    if (factorial_num == -1) {
        printf("Input error occurred.\n");
        return 1;
    }
    
    int series_terms = get_valid_int("Enter number of terms for series sum (1-100): ", 1, 100);
    if (series_terms == -1) {
        printf("Input error occurred.\n");
        return 1;
    }
    
    printf("\nResults:\n");
    printf("Square root of %.2f: %.6f\n", number, compute_sqrt_newton(number));
    printf("%.2f raised to power %d: %.6f\n", number, exponent, compute_power(number, exponent));
    printf("Factorial of %d: %.0f\n", factorial_num, compute_factorial(factorial_num));
    printf("Sum of first %d terms of 1/n^2 series: %.10f\n", series_terms, compute_series_sum(series_terms));
    
    return 0;
}