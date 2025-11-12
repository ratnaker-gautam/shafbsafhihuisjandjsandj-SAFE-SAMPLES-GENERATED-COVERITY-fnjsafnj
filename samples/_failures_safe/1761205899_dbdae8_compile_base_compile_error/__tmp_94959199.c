//DeepSeek-V3 V2.5 Category: Safe ; Style: modular_functions ; Variation: numeric_computation
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#define MAX_ITERATIONS 1000
#define EPSILON 1e-10
#define PI 3.14159265358979323846

double compute_pi_approximation(int iterations) {
    if (iterations <= 0 || iterations > MAX_ITERATIONS) {
        return -1.0;
    }
    
    double sum = 0.0;
    for (int i = 0; i < iterations; i++) {
        double term = 1.0 / (2.0 * i + 1.0);
        if (i % 2 == 0) {
            sum += term;
        } else {
            sum -= term;
        }
    }
    return 4.0 * sum;
}

double compute_euler_number(int terms) {
    if (terms <= 0 || terms > MAX_ITERATIONS) {
        return -1.0;
    }
    
    double result = 1.0;
    double factorial = 1.0;
    
    for (int i = 1; i < terms; i++) {
        if (factorial > 1e308 / i) {
            return -1.0;
        }
        factorial *= i;
        result += 1.0 / factorial;
    }
    return result;
}

double compute_sqrt_newton(double number, int max_iter) {
    if (number < 0.0 || max_iter <= 0 || max_iter > MAX_ITERATIONS) {
        return -1.0;
    }
    
    if (number == 0.0) {
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

int get_validated_integer(const char* prompt, int min_val, int max_val) {
    printf("%s", prompt);
    
    char buffer[100];
    if (fgets(buffer, sizeof(buffer), stdin) == NULL) {
        return -1;
    }
    
    char* endptr;
    long value = strtol(buffer, &endptr, 10);
    
    if (endptr == buffer || *endptr != '\n') {
        return -1;
    }
    
    if (value < min_val || value > max_val) {
        return -1;
    }
    
    return (int)value;
}

double get_validated_double(const char* prompt, double min_val, double max_val) {
    printf("%s", prompt);
    
    char buffer[100];
    if (fgets(buffer, sizeof(buffer), stdin) == NULL) {
        return -1.0;
    }
    
    char* endptr;
    double value = strtod(buffer, &endptr);
    
    if (endptr == buffer || *endptr != '\n') {
        return -1.0;
    }
    
    if (value < min_val || value > max_val) {
        return -1.0;
    }
    
    return value;
}

void display_menu() {
    printf("\nNumeric Computation Menu:\n");
    printf("1. Approximate Pi using Leibniz formula\n");
    printf("2. Compute Euler's number\n");
    printf("3. Compute square root using Newton's method\n");
    printf("4. Exit\n");
    printf("Enter your choice (1-4): ");
}

int main() {
    printf("Numeric Computation Program\n");
    
    while (1) {
        display_menu();
        
        int choice = get_validated_integer("", 1, 4);
        if (choice == -1) {
            printf("Invalid input. Please enter a number between 1 and 4.\n");
            continue;
        }
        
        if (choice == 4) {
            printf("Goodbye!\n");
            break;
        }
        
        switch (choice) {
            case 1: {
                int iterations = get_validated_integer("Enter number of iterations (1-1000): ", 1, 1000);
                if (iterations == -1) {
                    printf("Invalid number of iterations.\n");
                    break;
                }
                double pi_approx = compute_pi_approximation(iterations);
                printf("Pi approximation with %d iterations: %.10f\n", iterations, pi_approx);
                printf("Difference from math.h PI: %.10f\n", fabs(pi_approx - PI));
                break;
            }
            
            case 2: {
                int terms = get_validated_integer("Enter number of terms (1-1000): ", 1, 1000);
                if (terms == -1) {
                    printf("Invalid number of terms.\n");
                    break;
                }
                double euler = compute_euler_number(terms);
                printf("Euler's number with %d terms: %.10f\n", terms, euler);
                break;
            }
            
            case 3: {
                double number = get_validated_double("Enter a positive number: ", 0.0, 1e308);
                if (number == -1.0) {
                    printf("Invalid number.\n");
                    break;
                }
                int iterations = get_validated_integer("Enter maximum iterations (1-1000): ", 1, 1000);
                if (iterations