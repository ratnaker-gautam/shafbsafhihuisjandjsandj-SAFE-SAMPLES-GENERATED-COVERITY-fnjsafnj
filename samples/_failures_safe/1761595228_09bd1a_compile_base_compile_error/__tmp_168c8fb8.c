//DeepSeek-V3 V2.5 Category: Safe ; Style: modular_functions ; Variation: numeric_computation
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#define MAX_ITERATIONS 1000
#define EPSILON 1e-10

double compute_sqrt(double x) {
    if (x < 0.0) {
        fprintf(stderr, "Error: Cannot compute square root of negative number\n");
        exit(EXIT_FAILURE);
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
    fprintf(stderr, "Error: Square root computation did not converge\n");
    exit(EXIT_FAILURE);
}

double compute_power(double base, int exponent) {
    if (exponent < 0) {
        fprintf(stderr, "Error: Negative exponents not supported\n");
        exit(EXIT_FAILURE);
    }
    double result = 1.0;
    for (int i = 0; i < exponent; i++) {
        if (result > 1e308 / base) {
            fprintf(stderr, "Error: Power computation would overflow\n");
            exit(EXIT_FAILURE);
        }
        result *= base;
    }
    return result;
}

double compute_factorial(int n) {
    if (n < 0) {
        fprintf(stderr, "Error: Factorial of negative number is undefined\n");
        exit(EXIT_FAILURE);
    }
    if (n > 20) {
        fprintf(stderr, "Error: Factorial too large for double precision\n");
        exit(EXIT_FAILURE);
    }
    double result = 1.0;
    for (int i = 2; i <= n; i++) {
        result *= i;
    }
    return result;
}

double compute_series_sum(int terms) {
    if (terms <= 0) {
        fprintf(stderr, "Error: Number of terms must be positive\n");
        exit(EXIT_FAILURE);
    }
    if (terms > 1000000) {
        fprintf(stderr, "Error: Too many terms requested\n");
        exit(EXIT_FAILURE);
    }
    double sum = 0.0;
    for (int i = 1; i <= terms; i++) {
        double term = 1.0 / (i * i);
        if (term < 1e-308) {
            fprintf(stderr, "Error: Term too small for double precision\n");
            exit(EXIT_FAILURE);
        }
        sum += term;
    }
    return sum;
}

void print_menu(void) {
    printf("Numeric Computation Menu:\n");
    printf("1. Compute square root\n");
    printf("2. Compute power\n");
    printf("3. Compute factorial\n");
    printf("4. Compute series sum (1/n^2)\n");
    printf("5. Exit\n");
    printf("Enter your choice (1-5): ");
}

int get_valid_int(const char* prompt, int min_val, int max_val) {
    printf("%s", prompt);
    int value;
    if (scanf("%d", &value) != 1) {
        fprintf(stderr, "Error: Invalid integer input\n");
        exit(EXIT_FAILURE);
    }
    if (value < min_val || value > max_val) {
        fprintf(stderr, "Error: Value out of range [%d, %d]\n", min_val, max_val);
        exit(EXIT_FAILURE);
    }
    return value;
}

double get_valid_double(const char* prompt, double min_val, double max_val) {
    printf("%s", prompt);
    double value;
    if (scanf("%lf", &value) != 1) {
        fprintf(stderr, "Error: Invalid floating-point input\n");
        exit(EXIT_FAILURE);
    }
    if (value < min_val || value > max_val) {
        fprintf(stderr, "Error: Value out of range [%g, %g]\n", min_val, max_val);
        exit(EXIT_FAILURE);
    }
    return value;
}

int main(void) {
    int choice;
    double result;
    
    while (1) {
        print_menu();
        choice = get_valid_int("", 1, 5);
        
        if (choice == 5) {
            printf("Goodbye!\n");
            break;
        }
        
        switch (choice) {
            case 1: {
                double num = get_valid_double("Enter number for square root: ", 0.0, 1e308);
                result = compute_sqrt(num);
                printf("Square root of %g is %g\n", num, result);
                break;
            }
            case 2: {
                double base = get_valid_double("Enter base: ", -1000.0, 1000.0);
                int exponent = get_valid_int("Enter exponent: ", 0, 1000);
                result = compute_power(base, exponent);
                printf("%g^%d = %g\n", base, exponent, result);
                break;
            }
            case 3: {
                int n = get_valid_int("Enter number for factorial: ", 0, 20);
                result = compute_factorial(n);
                printf("%d! = %g\n", n, result