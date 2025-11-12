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

double compute_integral_sine(double a, double b, int n) {
    if (n <= 0 || a >= b) {
        return NAN;
    }
    double h = (b - a) / n;
    double sum = 0.0;
    for (int i = 0; i < n; i++) {
        double x = a + (i + 0.5) * h;
        sum += sin(x);
    }
    return sum * h;
}

int validate_double_input(const char* input, double* result) {
    if (input == NULL || result == NULL) {
        return 0;
    }
    char* endptr;
    *result = strtod(input, &endptr);
    if (endptr == input || *endptr != '\0') {
        return 0;
    }
    return 1;
}

int validate_int_input(const char* input, int* result) {
    if (input == NULL || result == NULL) {
        return 0;
    }
    char* endptr;
    long val = strtol(input, &endptr, 10);
    if (endptr == input || *endptr != '\0' || val < 0 || val > 1000000) {
        return 0;
    }
    *result = (int)val;
    return 1;
}

void print_menu(void) {
    printf("Numeric Computation Menu:\n");
    printf("1. Compute square root using Newton's method\n");
    printf("2. Compute integral of sin(x) using midpoint rule\n");
    printf("3. Exit\n");
    printf("Enter your choice (1-3): ");
}

int main(void) {
    char buffer[256];
    int choice;
    
    while (1) {
        print_menu();
        
        if (fgets(buffer, sizeof(buffer), stdin) == NULL) {
            break;
        }
        
        if (!validate_int_input(buffer, &choice)) {
            printf("Invalid choice. Please enter 1, 2, or 3.\n");
            continue;
        }
        
        if (choice == 1) {
            printf("Enter a non-negative number: ");
            if (fgets(buffer, sizeof(buffer), stdin) == NULL) {
                continue;
            }
            double x;
            if (!validate_double_input(buffer, &x)) {
                printf("Invalid number.\n");
                continue;
            }
            double result = compute_sqrt_newton(x);
            if (isnan(result)) {
                printf("Cannot compute square root of negative number.\n");
            } else {
                printf("Square root of %.6f is %.10f\n", x, result);
                printf("Library sqrt: %.10f\n", sqrt(x));
            }
        } else if (choice == 2) {
            printf("Enter lower bound: ");
            if (fgets(buffer, sizeof(buffer), stdin) == NULL) {
                continue;
            }
            double a;
            if (!validate_double_input(buffer, &a)) {
                printf("Invalid lower bound.\n");
                continue;
            }
            
            printf("Enter upper bound: ");
            if (fgets(buffer, sizeof(buffer), stdin) == NULL) {
                continue;
            }
            double b;
            if (!validate_double_input(buffer, &b)) {
                printf("Invalid upper bound.\n");
                continue;
            }
            
            if (a >= b) {
                printf("Lower bound must be less than upper bound.\n");
                continue;
            }
            
            printf("Enter number of intervals (1-1000000): ");
            if (fgets(buffer, sizeof(buffer), stdin) == NULL) {
                continue;
            }
            int n;
            if (!validate_int_input(buffer, &n)) {
                printf("Invalid number of intervals.\n");
                continue;
            }
            
            double result = compute_integral_sine(a, b, n);
            if (isnan(result)) {
                printf("Invalid parameters for integration.\n");
            } else {
                printf("Integral of sin(x) from %.6f to %.6f is %.10f\n", a, b, result);
                double exact = cos(a) - cos(b);
                printf("Exact value: %.10f\n", exact);
            }
        } else if (choice == 3) {
            printf("Goodbye!\n");
            break;
        } else {
            printf("Invalid choice. Please enter 1, 2, or 3.\n");
        }
    }
    
    return 0;
}