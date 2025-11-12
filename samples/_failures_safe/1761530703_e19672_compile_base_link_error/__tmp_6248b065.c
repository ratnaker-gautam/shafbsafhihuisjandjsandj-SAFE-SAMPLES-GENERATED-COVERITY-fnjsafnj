//DeepSeek-V3 V2.5 Category: Safe ; Style: function_pointers ; Variation: numeric_computation
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <stdint.h>
#include <limits.h>

typedef double (*numeric_operation)(double, double);

double add(double a, double b) {
    return a + b;
}

double subtract(double a, double b) {
    return a - b;
}

double multiply(double a, double b) {
    return a * b;
}

double divide(double a, double b) {
    if (fabs(b) < 1e-15) {
        fprintf(stderr, "Error: Division by zero\n");
        exit(EXIT_FAILURE);
    }
    return a / b;
}

double power(double a, double b) {
    if (a < 0 && fmod(b, 1.0) != 0) {
        fprintf(stderr, "Error: Negative base with fractional exponent\n");
        exit(EXIT_FAILURE);
    }
    return pow(a, b);
}

double safe_input(const char* prompt) {
    char buffer[256];
    double value;
    
    while (1) {
        printf("%s", prompt);
        if (fgets(buffer, sizeof(buffer), stdin) == NULL) {
            fprintf(stderr, "Error reading input\n");
            exit(EXIT_FAILURE);
        }
        
        char* endptr;
        value = strtod(buffer, &endptr);
        
        if (endptr == buffer || *endptr != '\n') {
            printf("Invalid input. Please enter a valid number.\n");
            continue;
        }
        
        if (value == HUGE_VAL || value == -HUGE_VAL) {
            printf("Number out of range. Please try again.\n");
            continue;
        }
        
        break;
    }
    
    return value;
}

int main() {
    numeric_operation operations[] = {add, subtract, multiply, divide, power};
    const char* operation_names[] = {"Addition", "Subtraction", "Multiplication", "Division", "Power"};
    const int num_operations = sizeof(operations) / sizeof(operations[0]);
    
    printf("Numeric Computation Calculator\n");
    printf("Available operations:\n");
    for (int i = 0; i < num_operations; i++) {
        printf("%d. %s\n", i + 1, operation_names[i]);
    }
    
    int choice;
    while (1) {
        printf("Select operation (1-%d): ", num_operations);
        char buffer[256];
        if (fgets(buffer, sizeof(buffer), stdin) == NULL) {
            fprintf(stderr, "Error reading input\n");
            return EXIT_FAILURE;
        }
        
        char* endptr;
        long temp = strtol(buffer, &endptr, 10);
        
        if (endptr == buffer || *endptr != '\n' || temp < 1 || temp > num_operations) {
            printf("Invalid choice. Please enter a number between 1 and %d.\n", num_operations);
            continue;
        }
        
        choice = (int)temp;
        break;
    }
    
    double a = safe_input("Enter first number: ");
    double b = safe_input("Enter second number: ");
    
    numeric_operation op = operations[choice - 1];
    double result = op(a, b);
    
    printf("Result: %.6f\n", result);
    
    return EXIT_SUCCESS;
}