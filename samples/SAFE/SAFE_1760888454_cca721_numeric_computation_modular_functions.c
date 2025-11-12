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
        return 1.0 / compute_power(base, -exponent);
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

double compute_approximate_e(int terms) {
    if (terms < 1 || terms > 100) {
        return NAN;
    }
    double e = 1.0;
    for (int i = 1; i <= terms; i++) {
        double term = 1.0 / compute_factorial(i);
        e += term;
    }
    return e;
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

int main() {
    printf("Numeric Computation Demo\n");
    printf("=======================\n");
    
    double number = get_valid_double("Enter a number to compute square root (0-1000): ", 0.0, 1000.0);
    double sqrt_result = compute_sqrt_newton(number);
    printf("Square root of %.2f: %.6f\n", number, sqrt_result);
    
    double base = get_valid_double("Enter base for power computation (-10 to 10): ", -10.0, 10.0);
    int exponent = get_valid_int("Enter integer exponent (-10 to 10): ", -10, 10);
    double power_result = compute_power(base, exponent);
    printf("%.2f^%d = %.6f\n", base, exponent, power_result);
    
    int factorial_n = get_valid_int("Enter integer for factorial (0-20): ", 0, 20);
    double factorial_result = compute_factorial(factorial_n);
    printf("%d! = %.0f\n", factorial_n, factorial_result);
    
    int e_terms = get_valid_int("Enter number of terms for e approximation (1-20): ", 1, 20);
    double e_approx = compute_approximate_e(e_terms);
    printf("e approximation with %d terms: %.10f\n", e_terms, e_approx);
    
    printf("Mathematical constant e from math.h: %.10f\n", exp(1.0));
    
    return 0;
}