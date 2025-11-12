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

int read_integer(const char* prompt, int min_val, int max_val) {
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

double read_double(const char* prompt, double min_val, double max_val) {
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

void display_menu(void) {
    printf("Numeric Computation Menu:\n");
    printf("1. Compute square root (Newton's method)\n");
    printf("2. Compute power (integer exponent)\n");
    printf("3. Compute factorial\n");
    printf("4. Exit\n");
}

int main(void) {
    int choice;
    double result;
    
    while (1) {
        display_menu();
        choice = read_integer("Enter your choice (1-4): ", 1, 4);
        if (choice == -1) {
            printf("Invalid input. Please enter a number between 1 and 4.\n");
            while (getchar() != '\n');
            continue;
        }
        
        if (choice == 4) {
            printf("Goodbye!\n");
            break;
        }
        
        switch (choice) {
            case 1: {
                double num = read_double("Enter a non-negative number: ", 0.0, 1e308);
                if (isnan(num)) {
                    printf("Invalid input. Please enter a valid non-negative number.\n");
                    break;
                }
                result = compute_sqrt_newton(num);
                if (isnan(result)) {
                    printf("Error: Cannot compute square root of negative number.\n");
                } else {
                    printf("Square root of %.6f is %.10f\n", num, result);
                }
                break;
            }
            case 2: {
                double base = read_double("Enter base: ", -1e308, 1e308);
                if (isnan(base)) {
                    printf("Invalid base input.\n");
                    break;
                }
                int exp = read_integer("Enter non-negative integer exponent: ", 0, 1000);
                if (exp == -1) {
                    printf("Invalid exponent input.\n");
                    break;
                }
                result = compute_power(base, exp);
                if (isnan(result)) {
                    printf("Error: Invalid exponent.\n");
                } else if (isinf(result)) {
                    printf("Result exceeds representable range.\n");
                } else {
                    printf("%.6f^%d = %.10f\n", base, exp, result);
                }
                break;
            }
            case 3: {
                int n = read_integer("Enter non-negative integer (0-170): ", 0, 170);
                if (n == -1) {
                    printf("Invalid input. Please enter an integer between 0 and 170.\n");
                    break;
                }
                result = compute_factorial(n);
                if (isnan(result)) {
                    printf("Error: Cannot compute factorial of negative number.\n");
                } else if (isinf(result)) {
                    printf("Factorial exceeds representable range.\n");
                } else {
                    printf("%d! = %.0f\n", n, result);
                }
                break;
            }
        }
    }
    
    return 0;
}