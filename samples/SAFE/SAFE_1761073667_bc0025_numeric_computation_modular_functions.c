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
    if (n > 170) {
        return INFINITY;
    }
    double result = 1.0;
    for (int i = 2; i <= n; i++) {
        result *= i;
    }
    return result;
}

int validate_integer_input(const char* input, int* value) {
    char* endptr;
    long temp = strtol(input, &endptr, 10);
    if (endptr == input || *endptr != '\n' && *endptr != '\0') {
        return 0;
    }
    if (temp < -1000000 || temp > 1000000) {
        return 0;
    }
    *value = (int)temp;
    return 1;
}

int validate_double_input(const char* input, double* value) {
    char* endptr;
    double temp = strtod(input, &endptr);
    if (endptr == input || *endptr != '\n' && *endptr != '\0') {
        return 0;
    }
    if (temp < -1e100 || temp > 1e100) {
        return 0;
    }
    *value = temp;
    return 1;
}

void print_result(double result) {
    if (isnan(result)) {
        printf("Result: Invalid input\n");
    } else if (isinf(result)) {
        printf("Result: Overflow\n");
    } else {
        printf("Result: %.10g\n", result);
    }
}

int main() {
    char input[256];
    int choice;
    double num1, num2;
    int int_val;
    
    printf("Numeric Computation Menu:\n");
    printf("1. Square root (Newton's method)\n");
    printf("2. Power (integer exponent)\n");
    printf("3. Factorial\n");
    printf("Enter choice (1-3): ");
    
    if (fgets(input, sizeof(input), stdin) == NULL) {
        printf("Input error\n");
        return 1;
    }
    
    if (!validate_integer_input(input, &choice) || choice < 1 || choice > 3) {
        printf("Invalid choice\n");
        return 1;
    }
    
    switch (choice) {
        case 1:
            printf("Enter number for square root: ");
            if (fgets(input, sizeof(input), stdin) == NULL) {
                printf("Input error\n");
                return 1;
            }
            if (!validate_double_input(input, &num1)) {
                printf("Invalid number\n");
                return 1;
            }
            print_result(compute_sqrt_newton(num1));
            break;
            
        case 2:
            printf("Enter base: ");
            if (fgets(input, sizeof(input), stdin) == NULL) {
                printf("Input error\n");
                return 1;
            }
            if (!validate_double_input(input, &num1)) {
                printf("Invalid base\n");
                return 1;
            }
            printf("Enter exponent (non-negative integer): ");
            if (fgets(input, sizeof(input), stdin) == NULL) {
                printf("Input error\n");
                return 1;
            }
            if (!validate_integer_input(input, &int_val) || int_val < 0) {
                printf("Invalid exponent\n");
                return 1;
            }
            print_result(compute_power(num1, int_val));
            break;
            
        case 3:
            printf("Enter integer for factorial (0-170): ");
            if (fgets(input, sizeof(input), stdin) == NULL) {
                printf("Input error\n");
                return 1;
            }
            if (!validate_integer_input(input, &int_val) || int_val < 0 || int_val > 170) {
                printf("Invalid input\n");
                return 1;
            }
            print_result(compute_factorial(int_val));
            break;
    }
    
    return 0;
}