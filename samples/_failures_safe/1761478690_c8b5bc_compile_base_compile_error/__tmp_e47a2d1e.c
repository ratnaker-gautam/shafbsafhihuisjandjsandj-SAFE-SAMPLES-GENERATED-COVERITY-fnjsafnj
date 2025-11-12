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
        double term = compute_factorial(i);
        if (isnan(term)) {
            return NAN;
        }
        e += 1.0 / term;
    }
    return e;
}

int get_valid_int(const char* prompt, int min, int max) {
    printf("%s", prompt);
    int value;
    if (scanf("%d", &value) != 1) {
        return -1;
    }
    if (value < min || value > max) {
        return -1;
    }
    return value;
}

double get_valid_double(const char* prompt, double min, double max) {
    printf("%s", prompt);
    double value;
    if (scanf("%lf", &value) != 1) {
        return NAN;
    }
    if (value < min || value > max) {
        return NAN;
    }
    return value;
}

void clear_input_buffer(void) {
    int c;
    while ((c = getchar()) != '\n' && c != EOF) {}
}

int main(void) {
    int choice;
    
    do {
        printf("\nNumeric Computation Menu:\n");
        printf("1. Compute square root (Newton's method)\n");
        printf("2. Compute power (integer exponent)\n");
        printf("3. Compute factorial\n");
        printf("4. Approximate e (Taylor series)\n");
        printf("5. Exit\n");
        printf("Enter your choice (1-5): ");
        
        if (scanf("%d", &choice) != 1) {
            clear_input_buffer();
            printf("Invalid input. Please enter a number.\n");
            continue;
        }
        
        clear_input_buffer();
        
        switch (choice) {
            case 1: {
                double num = get_valid_double("Enter a non-negative number: ", 0.0, 1e6);
                if (isnan(num)) {
                    printf("Invalid input.\n");
                    break;
                }
                double result = compute_sqrt_newton(num);
                if (isnan(result)) {
                    printf("Error computing square root.\n");
                } else {
                    printf("Square root of %.6f is %.10f\n", num, result);
                }
                break;
            }
            
            case 2: {
                double base = get_valid_double("Enter base: ", -100.0, 100.0);
                if (isnan(base)) {
                    printf("Invalid base.\n");
                    break;
                }
                int exp = get_valid_int("Enter non-negative integer exponent: ", 0, 20);
                if (exp == -1) {
                    printf("Invalid exponent.\n");
                    break;
                }
                double result = compute_power(base, exp);
                if (isnan(result)) {
                    printf("Error computing power.\n");
                } else {
                    printf("%.6f^%d = %.10f\n", base, exp, result);
                }
                break;
            }
            
            case 3: {
                int n = get_valid_int("Enter non-negative integer (0-20): ", 0, 20);
                if (n == -1) {
                    printf("Invalid input.\n");
                    break;
                }
                double result = compute_factorial(n);
                if (isnan(result)) {
                    printf("Error computing factorial.\n");
                } else {
                    printf("%d! = %.0f\n", n, result);
                }
                break;
            }
            
            case 4: {
                int terms = get_valid_int("Enter number of terms (1-100): ", 1, 100);
                if (terms == -1) {
                    printf("Invalid input.\n");
                    break;
                }
                double result = compute_approximate_e(terms);
                if (isnan(result)) {
                    printf("Error computing e approximation.\n");
                } else {
                    printf("e approximation with %d terms: %.10f\n", terms, result);
                }
                break;