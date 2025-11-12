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
        if (result > __DBL_MAX__ / base) {
            return INFINITY;
        }
        result *= base;
    }
    return result;
}

double compute_geometric_mean(const double* numbers, int count) {
    if (count <= 0 || numbers == NULL) {
        return NAN;
    }
    double product = 1.0;
    for (int i = 0; i < count; i++) {
        if (numbers[i] <= 0.0) {
            return NAN;
        }
        if (product > __DBL_MAX__ / numbers[i]) {
            return INFINITY;
        }
        product *= numbers[i];
    }
    return pow(product, 1.0 / count);
}

int read_numbers(double* buffer, int max_count) {
    if (buffer == NULL || max_count <= 0) {
        return -1;
    }
    printf("Enter numbers (up to %d, 0 to stop): ", max_count);
    int count = 0;
    while (count < max_count) {
        double value;
        int result = scanf("%lf", &value);
        if (result != 1) {
            while (getchar() != '\n');
            printf("Invalid input. Please enter a number.\n");
            continue;
        }
        if (value == 0.0) {
            break;
        }
        buffer[count] = value;
        count++;
    }
    return count;
}

void print_menu() {
    printf("\nNumeric Computation Menu:\n");
    printf("1. Compute square root (Newton's method)\n");
    printf("2. Compute power (integer exponent)\n");
    printf("3. Compute geometric mean\n");
    printf("4. Exit\n");
    printf("Enter your choice: ");
}

int main() {
    const int MAX_NUMBERS = 10;
    double numbers[MAX_NUMBERS];
    
    while (1) {
        print_menu();
        int choice;
        if (scanf("%d", &choice) != 1) {
            while (getchar() != '\n');
            printf("Invalid input. Please enter a number.\n");
            continue;
        }
        
        switch (choice) {
            case 1: {
                printf("Enter a non-negative number: ");
                double x;
                if (scanf("%lf", &x) != 1) {
                    while (getchar() != '\n');
                    printf("Invalid input.\n");
                    break;
                }
                double result = compute_sqrt_newton(x);
                if (isnan(result)) {
                    printf("Error: Cannot compute square root of negative number.\n");
                } else {
                    printf("Square root of %.2f is %.6f\n", x, result);
                }
                break;
            }
            
            case 2: {
                printf("Enter base: ");
                double base;
                if (scanf("%lf", &base) != 1) {
                    while (getchar() != '\n');
                    printf("Invalid input.\n");
                    break;
                }
                printf("Enter non-negative integer exponent: ");
                int exponent;
                if (scanf("%d", &exponent) != 1) {
                    while (getchar() != '\n');
                    printf("Invalid input.\n");
                    break;
                }
                double result = compute_power(base, exponent);
                if (isnan(result)) {
                    printf("Error: Negative exponent not supported.\n");
                } else if (isinf(result)) {
                    printf("Error: Result overflow.\n");
                } else {
                    printf("%.2f^%d = %.6f\n", base, exponent, result);
                }
                break;
            }
            
            case 3: {
                int count = read_numbers(numbers, MAX_NUMBERS);
                if (count <= 0) {
                    printf("No valid numbers entered.\n");
                    break;
                }
                double result = compute_geometric_mean(numbers, count);
                if (isnan(result)) {
                    printf("Error: Cannot compute geometric mean of non-positive numbers.\n");
                } else if (isinf(result)) {
                    printf("Error: Result overflow.\n");
                } else {
                    printf("Geometric mean: %.6f\n", result);
                }
                break;
            }
            
            case 4:
                printf("Goodbye!\n");
                return 0;
                
            default:
                printf("Invalid choice. Please try again.\n");
                break;
        }
    }
    
    return 0