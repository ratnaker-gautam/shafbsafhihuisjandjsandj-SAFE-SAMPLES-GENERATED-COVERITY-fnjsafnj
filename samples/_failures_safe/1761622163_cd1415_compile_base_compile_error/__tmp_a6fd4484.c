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
        sum += 1.0 / compute_power(2, i);
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

void display_results(double sqrt_result, double power_result, double factorial_result, double series_result) {
    printf("\nComputation Results:\n");
    printf("Square root: ");
    if (isnan(sqrt_result)) {
        printf("Invalid input (negative number)\n");
    } else {
        printf("%.6f\n", sqrt_result);
    }
    
    printf("Power result: ");
    if (isnan(power_result)) {
        printf("Invalid input (negative exponent)\n");
    } else {
        printf("%.6f\n", power_result);
    }
    
    printf("Factorial: ");
    if (isnan(factorial_result)) {
        printf("Invalid input (negative number)\n");
    } else {
        printf("%.0f\n", factorial_result);
    }
    
    printf("Series sum: ");
    if (isnan(series_result)) {
        printf("Invalid input (non-positive terms)\n");
    } else {
        printf("%.6f\n", series_result);
    }
}

int main() {
    printf("Numeric Computation Program\n");
    printf("===========================\n\n");
    
    double number = get_valid_double("Enter a positive number for square root calculation (0.1 to 1000.0): ", 0.1, 1000.0);
    if (isnan(number)) {
        printf("Error reading input.\n");
        return 1;
    }
    
    double base = get_valid_double("Enter base for power calculation (0.1 to 10.0): ", 0.1, 10.0);
    if (isnan(base)) {
        printf("Error reading input.\n");
        return 1;
    }
    
    int exponent = get_valid_int("Enter exponent for power calculation (0 to 20): ", 0, 20);
    if (exponent == -1) {
        printf("Error reading input.\n");
        return 1;
    }
    
    int factorial_input = get_valid_int("Enter number for factorial calculation (0 to 20): ", 0, 20);
    if (factorial_input == -1) {
        printf("Error reading input.\n");
        return 1;
    }
    
    int series_terms = get_valid_int("Enter number of terms for series sum (1 to 50): ", 1, 50);
    if (series_terms == -1) {
        printf("Error reading input.\n");
        return 1;
    }
    
    double sqrt_result = compute_sqrt_newton(number);
    double power_result = compute_power(base, exponent);
    double factorial_result = compute_factorial(factorial