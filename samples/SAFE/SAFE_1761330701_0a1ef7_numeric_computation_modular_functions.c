//DeepSeek-V3 V2.5 Category: Safe ; Style: modular_functions ; Variation: numeric_computation
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#define MAX_ITERATIONS 1000
#define EPSILON 1e-10

double compute_sqrt(double x) {
    if (x < 0.0) {
        return -1.0;
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
        return -1.0;
    }
    double result = 1.0;
    for (int i = 0; i < exponent; i++) {
        if (result > 1e308 / base) {
            return -1.0;
        }
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
        if (result > 1e308 / i) {
            return -1.0;
        }
        result *= i;
    }
    return result;
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

void display_results(double sqrt_result, double power_result, double fact_result) {
    printf("\nComputation Results:\n");
    printf("Square root: ");
    if (sqrt_result >= 0.0) {
        printf("%.6f\n", sqrt_result);
    } else {
        printf("Invalid input (negative number)\n");
    }
    
    printf("Power: ");
    if (power_result >= 0.0) {
        printf("%.6f\n", power_result);
    } else {
        printf("Invalid input (negative exponent or overflow)\n");
    }
    
    printf("Factorial: ");
    if (fact_result >= 0.0) {
        printf("%.0f\n", fact_result);
    } else {
        printf("Invalid input (negative number or overflow)\n");
    }
}

int main() {
    printf("Numeric Computation Program\n");
    printf("===========================\n");
    
    double number = get_valid_double("Enter a positive number for square root calculation (0-1000): ", 0.0, 1000.0);
    if (number < 0.0) {
        printf("Input error occurred.\n");
        return 1;
    }
    
    double base = get_valid_double("Enter base for power calculation (0-100): ", 0.0, 100.0);
    if (base < 0.0) {
        printf("Input error occurred.\n");
        return 1;
    }
    
    int exponent = get_valid_int("Enter exponent for power calculation (0-20): ", 0, 20);
    if (exponent < 0) {
        printf("Input error occurred.\n");
        return 1;
    }
    
    int factorial_num = get_valid_int("Enter number for factorial calculation (0-170): ", 0, 170);
    if (factorial_num < 0) {
        printf("Input error occurred.\n");
        return 1;
    }
    
    double sqrt_result = compute_sqrt(number);
    double power_result = compute_power(base, exponent);
    double fact_result = compute_factorial(factorial_num);
    
    display_results(sqrt_result, power_result, fact_result);
    
    return 0;
}