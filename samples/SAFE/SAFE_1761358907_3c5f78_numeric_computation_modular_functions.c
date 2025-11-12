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
        sum += 1.0 / compute_power(i, 2);
    }
    return sum;
}

int get_valid_int(const char* prompt, int min_val, int max_val) {
    int value;
    char buffer[100];
    while (1) {
        printf("%s", prompt);
        if (fgets(buffer, sizeof(buffer), stdin) == NULL) {
            return min_val;
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
            return min_val;
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
    printf("Square root: %.10f\n", sqrt_result);
    printf("Power: %.10f\n", power_result);
    printf("Factorial: %.10f\n", factorial_result);
    printf("Series sum: %.10f\n", series_result);
}

int main() {
    printf("Numeric Computation Program\n");
    printf("===========================\n\n");
    
    double number = get_valid_double("Enter a positive number for square root calculation (0.1 to 1000.0): ", 0.1, 1000.0);
    int exponent = get_valid_int("Enter exponent for power calculation (0 to 20): ", 0, 20);
    int factorial_num = get_valid_int("Enter number for factorial calculation (0 to 170): ", 0, 170);
    int series_terms = get_valid_int("Enter number of terms for series sum (1 to 1000): ", 1, 1000);
    
    double sqrt_result = compute_sqrt_newton(number);
    double power_result = compute_power(number, exponent);
    double factorial_result = compute_factorial(factorial_num);
    double series_result = compute_series_sum(series_terms);
    
    display_results(sqrt_result, power_result, factorial_result, series_result);
    
    return 0;
}