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

int validate_integer_input(const char* input, int* result) {
    if (input == NULL || result == NULL) {
        return 0;
    }
    
    char* endptr;
    long value = strtol(input, &endptr, 10);
    
    if (endptr == input || *endptr != '\0') {
        return 0;
    }
    
    if (value < -1000000 || value > 1000000) {
        return 0;
    }
    
    *result = (int)value;
    return 1;
}

int validate_double_input(const char* input, double* result) {
    if (input == NULL || result == NULL) {
        return 0;
    }
    
    char* endptr;
    double value = strtod(input, &endptr);
    
    if (endptr == input || *endptr != '\0') {
        return 0;
    }
    
    if (value < -1e100 || value > 1e100) {
        return 0;
    }
    
    *result = value;
    return 1;
}

void display_menu(void) {
    printf("Numeric Computation Menu:\n");
    printf("1. Approximate Pi using Leibniz formula\n");
    printf("2. Compute square root using Newton's method\n");
    printf("3. Exit\n");
    printf("Enter your choice (1-3): ");
}

int main(void) {
    char input_buffer[100];
    int choice;
    
    while (1) {
        display_menu();
        
        if (fgets(input_buffer, sizeof(input_buffer), stdin) == NULL) {
            printf("Input error occurred.\n");
            continue;
        }
        
        if (!validate_integer_input(input_buffer, &choice)) {
            printf("Invalid input. Please enter a valid integer.\n");
            continue;
        }
        
        switch (choice) {
            case 1: {
                printf("Enter number of iterations (1-%d): ", MAX_ITERATIONS);
                if (fgets(input_buffer, sizeof(input_buffer), stdin) == NULL) {
                    printf("Input error occurred.\n");
                    break;
                }
                
                int iterations;
                if (!validate_integer_input(input_buffer, &iterations)) {
                    printf("Invalid iteration count.\n");
                    break;
                }
                
                double pi_approx = compute_pi_approximation(iterations);
                if (pi_approx < 0) {
                    printf("Invalid iteration count.\n");
                } else {
                    printf("Pi approximation: %.10f\n", pi_approx);
                    printf("Difference from math.h PI: %.10f\n", fabs(pi_approx - PI));
                }
                break;
            }
            
            case 2: {
                printf("Enter a positive number: ");
                if (fgets(input_buffer, sizeof(input_buffer), stdin) == NULL) {
                    printf("Input error occurred.\n");
                    break;
                }
                
                double number;
                if (!validate_double_input(input_buffer, &number)) {
                    printf("Invalid number.\n");
                    break;
                }
                
                printf("Enter maximum iterations (1-%d): ", MAX_ITERATIONS);
                if (fgets(input_buffer, sizeof(input_buffer), stdin) == NULL) {
                    printf("Input error occurred.\n");
                    break;
                }
                
                int max_iter;
                if (!validate_integer_input(input_buffer, &max_iter)) {
                    printf("Invalid iteration count.\n");
                    break;
                }
                
                double sqrt_result = compute_sqrt_newton(number, max_iter);
                if (sqrt_result < 0) {
                    printf("Invalid input parameters.\n");
                } else {
                    double math_sqrt = sqrt(number);
                    printf("Square root: %.10f\n", sqrt_result);
                    printf("Difference from math.h sqrt: %.10f\n", fabs(sqrt_result - math_sqrt));
                }
                break;
            }
            
            case 3:
                printf("Exiting program.\n");
                return 0;
            
            default:
                printf("Invalid choice. Please enter 1, 2