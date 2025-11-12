//DeepSeek-V3 V2.5 Category: Safe ; Style: modular_functions ; Variation: numeric_computation
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#define MAX_ITERATIONS 1000
#define EPSILON 1e-10

double compute_sqrt_newton(double n) {
    if (n < 0.0) {
        return -1.0;
    }
    if (n == 0.0) {
        return 0.0;
    }
    double x = n;
    double prev;
    int iter = 0;
    do {
        prev = x;
        x = 0.5 * (x + n / x);
        iter++;
    } while (fabs(x - prev) > EPSILON && iter < MAX_ITERATIONS);
    return x;
}

double compute_power(double base, int exponent) {
    if (exponent < 0) {
        return -1.0;
    }
    double result = 1.0;
    for (int i = 0; i < exponent; i++) {
        result *= base;
    }
    return result;
}

double compute_factorial(int n) {
    if (n < 0) {
        return -1.0;
    }
    double result = 1.0;
    for (int i = 2; i <= n; i++) {
        result *= i;
    }
    return result;
}

double compute_series_sum(int terms) {
    if (terms <= 0) {
        return -1.0;
    }
    double sum = 0.0;
    for (int i = 1; i <= terms; i++) {
        double term = 1.0 / compute_power(i, 2);
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
            return -1.0;
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
    
    double num = get_valid_double("Enter a number to compute square root (0-1000): ", 0.0, 1000.0);
    if (num < 0.0) {
        return 1;
    }
    double sqrt_result = compute_sqrt_newton(num);
    printf("Square root of %.2f: %.6f\n", num, sqrt_result);
    
    int exp = get_valid_int("Enter exponent for power calculation (0-20): ", 0, 20);
    if (exp < 0) {
        return 1;
    }
    double power_result = compute_power(num, exp);
    printf("%.2f^%d: %.6f\n", num, exp, power_result);
    
    int fact_num = get_valid_int("Enter number for factorial (0-20): ", 0, 20);
    if (fact_num < 0) {
        return 1;
    }
    double fact_result = compute_factorial(fact_num);
    printf("%d! = %.0f\n", fact_num, fact_result);
    
    int series_terms = get_valid_int("Enter number of terms for series sum (1-100): ", 1, 100);
    if (series_terms < 0) {
        return 1;
    }
    double series_result = compute_series_sum(series_terms);
    printf("Series sum (1/n^2) with %d terms: %.6f\n", series_terms, series_result);
    
    return 0;
}