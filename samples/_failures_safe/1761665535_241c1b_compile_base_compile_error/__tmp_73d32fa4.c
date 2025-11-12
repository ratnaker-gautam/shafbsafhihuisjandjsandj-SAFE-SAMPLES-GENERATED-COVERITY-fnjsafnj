//DeepSeek-V3 V2.5 Category: Safe ; Style: modular_functions ; Variation: numeric_computation
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#define MAX_ITERATIONS 1000
#define EPSILON 1e-10
#define PI 3.14159265358979323846

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

double compute_sqrt_newton(double number) {
    if (number < 0.0) {
        return -1.0;
    }
    if (fabs(number) < EPSILON) {
        return 0.0;
    }
    
    double guess = number / 2.0;
    for (int i = 0; i < MAX_ITERATIONS; i++) {
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

int get_valid_integer(const char* prompt, int min, int max) {
    int value;
    char buffer[100];
    
    while (1) {
        printf("%s", prompt);
        if (fgets(buffer, sizeof(buffer), stdin) == NULL) {
            return -1;
        }
        
        if (sscanf(buffer, "%d", &value) == 1) {
            if (value >= min && value <= max) {
                return value;
            }
        }
        printf("Invalid input. Please enter an integer between %d and %d.\n", min, max);
    }
}

double get_valid_double(const char* prompt, double min, double max) {
    double value;
    char buffer[100];
    
    while (1) {
        printf("%s", prompt);
        if (fgets(buffer, sizeof(buffer), stdin) == NULL) {
            return -1.0;
        }
        
        if (sscanf(buffer, "%lf", &value) == 1) {
            if (value >= min && value <= max) {
                return value;
            }
        }
        printf("Invalid input. Please enter a number between %.2f and %.2f.\n", min, max);
    }
}

void display_menu() {
    printf("\nNumeric Computation Menu:\n");
    printf("1. Approximate Pi using Leibniz formula\n");
    printf("2. Compute square root using Newton's method\n");
    printf("3. Compute factorial\n");
    printf("4. Exit\n");
}

int main() {
    printf("Numeric Computation Program\n");
    
    while (1) {
        display_menu();
        int choice = get_valid_integer("Enter your choice (1-4): ", 1, 4);
        
        if (choice == 1) {
            int terms = get_valid_integer("Enter number of terms (1-1000): ", 1, 1000);
            double pi_approx = compute_pi_approximation(terms);
            printf("Pi approximation with %d terms: %.10f\n", terms, pi_approx);
            printf("Actual Pi: %.10f\n", PI);
            printf("Error: %.10f\n", fabs(PI - pi_approx));
        }
        else if (choice == 2) {
            double number = get_valid_double("Enter a positive number: ", 0.0, 1e6);
            double sqrt_result = compute_sqrt_newton(number);
            printf("Square root of %.6f: %.10f\n", number, sqrt_result);
            printf("Built-in sqrt: %.10f\n", sqrt(number));
            printf("Difference: %.10f\n", fabs(sqrt_result - sqrt(number)));
        }
        else if (choice == 3) {
            int n = get_valid_integer("Enter an integer (0-20): ", 0, 20);
            double fact = compute_factorial(n);
            printf("%d! = %.0f\n", n, fact);
        }
        else if (choice == 4) {
            printf("Goodbye!\n");
            break;
        }
    }
    
    return 0;
}