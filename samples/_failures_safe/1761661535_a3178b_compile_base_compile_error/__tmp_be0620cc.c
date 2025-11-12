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
    if (number == 0.0) {
        return 0.0;
    }
    
    double guess = number / 2.0;
    for (int i = 0; i < MAX_ITERATIONS; i++) {
        if (fabs(guess) < EPSILON) {
            return 0.0;
        }
        double new_guess = 0.5 * (guess + number / guess);
        if (fabs(new_guess - guess) < EPSILON) {
            return new_guess;
        }
        guess = new_guess;
    }
    return guess;
}

int validate_integer_input(const char* input, int* value) {
    if (input == NULL || value == NULL) {
        return 0;
    }
    
    char* endptr;
    long temp = strtol(input, &endptr, 10);
    
    if (endptr == input || *endptr != '\0') {
        return 0;
    }
    if (temp < -1000000 || temp > 1000000) {
        return 0;
    }
    
    *value = (int)temp;
    return 1;
}

int validate_double_input(const char* input, double* value) {
    if (input == NULL || value == NULL) {
        return 0;
    }
    
    char* endptr;
    double temp = strtod(input, &endptr);
    
    if (endptr == input || *endptr != '\0') {
        return 0;
    }
    if (temp < -1e100 || temp > 1e100) {
        return 0;
    }
    
    *value = temp;
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
            printf("Error reading input.\n");
            continue;
        }
        
        if (!validate_integer_input(input_buffer, &choice)) {
            printf("Invalid choice. Please enter 1, 2, or 3.\n");
            continue;
        }
        
        switch (choice) {
            case 1: {
                printf("Enter number of terms (1-%d): ", MAX_ITERATIONS);
                if (fgets(input_buffer, sizeof(input_buffer), stdin) == NULL) {
                    printf("Error reading input.\n");
                    break;
                }
                
                int terms;
                if (!validate_integer_input(input_buffer, &terms)) {
                    printf("Invalid number of terms.\n");
                    break;
                }
                
                double pi_approx = compute_pi_approximation(terms);
                if (pi_approx < 0.0) {
                    printf("Invalid number of terms.\n");
                } else {
                    printf("Pi approximation with %d terms: %.10f\n", terms, pi_approx);
                    printf("Difference from actual Pi: %.10f\n", fabs(pi_approx - PI));
                }
                break;
            }
            
            case 2: {
                printf("Enter a positive number: ");
                if (fgets(input_buffer, sizeof(input_buffer), stdin) == NULL) {
                    printf("Error reading input.\n");
                    break;
                }
                
                double number;
                if (!validate_double_input(input_buffer, &number)) {
                    printf("Invalid number.\n");
                    break;
                }
                
                double sqrt_result = compute_sqrt_newton(number);
                if (sqrt_result < 0.0) {
                    printf("Cannot compute square root of negative number.\n");
                } else {
                    printf("Square root of %.6f: %.10f\n", number, sqrt_result);
                    double actual_sqrt = sqrt(number);
                    printf("Difference from sqrt(): %.10f\n", fabs(sqrt_result - actual_sqrt));
                }
                break;
            }
            
            case 3:
                printf("Goodbye!\n");
                return 0;
            
            default:
                printf("Invalid choice. Please enter 1, 2, or 3.\n");
                break;
        }
        
        printf("\n");
    }
    
    return 0;
}