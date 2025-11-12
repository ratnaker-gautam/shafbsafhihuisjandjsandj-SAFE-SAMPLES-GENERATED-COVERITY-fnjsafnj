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

void display_menu() {
    printf("\nNumeric Computation Menu:\n");
    printf("1. Compute square root\n");
    printf("2. Compute power\n");
    printf("3. Compute factorial\n");
    printf("4. Exit\n");
    printf("Enter your choice (1-4): ");
}

int main() {
    printf("Numeric Computation Program\n");
    
    while (1) {
        display_menu();
        int choice = get_valid_int("", 1, 4);
        
        if (choice == 1) {
            double num = get_valid_double("Enter a non-negative number: ", 0.0, 1e10);
            double result = compute_sqrt(num);
            if (result >= 0.0) {
                printf("Square root of %.2f is %.6f\n", num, result);
            } else {
                printf("Error: Cannot compute square root of negative number.\n");
            }
        }
        else if (choice == 2) {
            double base = get_valid_double("Enter base: ", -100.0, 100.0);
            int exponent = get_valid_int("Enter non-negative exponent: ", 0, 100);
            double result = compute_power(base, exponent);
            if (result >= 0.0) {
                printf("%.2f^%d = %.6f\n", base, exponent, result);
            } else {
                printf("Error: Overflow occurred in power computation.\n");
            }
        }
        else if (choice == 3) {
            int n = get_valid_int("Enter non-negative integer: ", 0, 170);
            double result = compute_factorial(n);
            if (result >= 0.0) {
                printf("%d! = %.0f\n", n, result);
            } else {
                printf("Error: Overflow occurred in factorial computation.\n");
            }
        }
        else if (choice == 4) {
            printf("Goodbye!\n");
            break;
        }
    }
    
    return 0;
}