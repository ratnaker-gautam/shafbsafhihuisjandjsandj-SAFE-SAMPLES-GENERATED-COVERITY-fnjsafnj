//DeepSeek-V3 V2.5 Category: Safe ; Style: function_pointers ; Variation: numeric_computation
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <stdint.h>
#include <limits.h>

typedef double (*numeric_func)(double, double);

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
    if (fabs(b) < 1e-12) {
        return NAN;
    }
    return a / b;
}

double power(double a, double b) {
    if (a < 0 && fmod(b, 1.0) != 0) {
        return NAN;
    }
    return pow(a, b);
}

double safe_input(const char* prompt) {
    char buffer[256];
    double value;
    
    while (1) {
        printf("%s", prompt);
        if (fgets(buffer, sizeof(buffer), stdin) == NULL) {
            exit(EXIT_FAILURE);
        }
        
        char* endptr;
        value = strtod(buffer, &endptr);
        
        if (endptr != buffer && *endptr == '\n') {
            break;
        }
        
        printf("Invalid input. Please enter a valid number.\n");
    }
    
    return value;
}

int main() {
    numeric_func operations[] = {add, subtract, multiply, divide, power};
    const char* operation_names[] = {"Addition", "Subtraction", "Multiplication", "Division", "Exponentiation"};
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
            return EXIT_FAILURE;
        }
        
        char* endptr;
        long temp = strtol(buffer, &endptr, 10);
        
        if (endptr != buffer && *endptr == '\n' && temp >= 1 && temp <= num_operations) {
            choice = (int)temp;
            break;
        }
        
        printf("Invalid choice. Please enter a number between 1 and %d.\n", num_operations);
    }
    
    double a = safe_input("Enter first number: ");
    double b = safe_input("Enter second number: ");
    
    numeric_func selected_func = operations[choice - 1];
    double result = selected_func(a, b);
    
    printf("\nOperation: %s\n", operation_names[choice - 1]);
    printf("Inputs: %.6f and %.6f\n", a, b);
    
    if (isnan(result)) {
        printf("Result: Undefined (mathematical error)\n");
    } else if (isinf(result)) {
        printf("Result: Infinity\n");
    } else {
        printf("Result: %.6f\n", result);
    }
    
    return 0;
}