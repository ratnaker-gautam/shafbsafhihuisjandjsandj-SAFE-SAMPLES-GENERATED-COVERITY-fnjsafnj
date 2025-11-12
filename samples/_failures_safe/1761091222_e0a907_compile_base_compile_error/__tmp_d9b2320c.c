//DeepSeek-V3 SAFE v1.4 Category: Safe ; Style: modular_functions ; Variation: numeric_computation
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#define MAX_ITERATIONS 1000
#define EPSILON 1e-10

double compute_pi_approximation(int terms) {
    if (terms <= 0 || terms > MAX_ITERATIONS) {
        return -1.0;
    }
    
    double sum = 0.0;
    for (int i = 0; i < terms; i++) {
        double term = 1.0 / (2.0 * i + 1.0);
        if (i % 2 == 0) {
            sum += term;
        } else {
            sum -= term;
        }
    }
    return 4.0 * sum;
}

double compute_sqrt_newton(double number, int max_iter) {
    if (number < 0.0 || max_iter <= 0 || max_iter > MAX_ITERATIONS) {
        return -1.0;
    }
    
    if (fabs(number) < EPSILON) {
        return 0.0;
    }
    
    double guess = number / 2.0;
    for (int i = 0; i < max_iter; i++) {
        double new_guess = 0.5 * (guess + number / guess);
        if (fabs(new_guess - guess) < EPSILON) {
            return new_guess;
        }
        guess = new_guess;
    }
    return guess;
}

double compute_factorial(int n) {
    if (n < 0 || n > 20) {
        return -1.0;
    }
    
    double result = 1.0;
    for (int i = 2; i <= n; i++) {
        result *= i;
    }
    return result;
}

double compute_exponential(double x, int terms) {
    if (terms <= 0 || terms > MAX_ITERATIONS) {
        return -1.0;
    }
    
    double result = 1.0;
    double term = 1.0;
    
    for (int i = 1; i < terms; i++) {
        term *= x / i;
        result += term;
    }
    return result;
}

int get_valid_integer(const char* prompt, int min_val, int max_val) {
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
    printf("1. Approximate Pi using Leibniz formula\n");
    printf("2. Compute square root using Newton's method\n");
    printf("3. Compute factorial\n");
    printf("4. Compute exponential using Taylor series\n");
    printf("5. Exit\n");
    printf("Enter your choice (1-5): ");
}

int main() {
    printf("Numeric Computation Program\n");
    printf("===========================\n");
    
    while (1) {
        display_menu();
        
        int choice = get_valid_integer("", 1, 5);
        if (choice == -1) {
            break;
        }
        
        switch (choice) {
            case 1: {
                int terms = get_valid_integer("Enter number of terms (1-1000): ", 1, 1000);
                if (terms != -1) {
                    double pi_approx = compute_pi_approximation(terms);
                    printf("Pi approximation with %d terms: %.10f\n", terms, pi_approx);
                    printf("Actual Pi: %.10f\n", 3.1415926535);
                }
                break;
            }
            case 2: {
                double number = get_valid_double("Enter number (0-1000): ", 0.0, 1000.0);
                int iterations = get_valid_integer("Enter maximum iterations (1-1000): ", 1, 1000);
                if (number != -1.0 && iterations != -1) {
                    double sqrt_result = compute_sqrt_newton(number, iterations);
                    printf("Square root of %.2f: %.10f\n", number, sqrt_result);
                    printf("Library sqrt: %.10f\n", sqrt(number));
                }
                break;
            }
            case 3: {
                int n = get_valid_integer("Enter integer (0-20): ", 0, 20);
                if (n != -1) {
                    double fact = compute_factorial(n);
                    printf("%d! = %.0f\n", n