//DeepSeek-V3 SAFE v1.4 Category: Safe ; Style: function_pointers ; Variation: numeric_computation
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

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
    if (fabs(b) < 1e-12) {
        printf("Error: Division by zero\n");
        exit(1);
    }
    return a / b;
}

double power(double a, double b) {
    if (a < 0 && fmod(b, 1.0) != 0) {
        printf("Error: Negative base with fractional exponent\n");
        exit(1);
    }
    return pow(a, b);
}

double safe_input(const char* prompt) {
    char buffer[100];
    double value;
    
    while (1) {
        printf("%s", prompt);
        if (fgets(buffer, sizeof(buffer), stdin) == NULL) {
            printf("Error reading input\n");
            exit(1);
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
    numeric_operation operations[] = {add, subtract, multiply, divide, power};
    const char* operation_names[] = {"addition", "subtraction", "multiplication", "division", "exponentiation"};
    int num_operations = sizeof(operations) / sizeof(operations[0]);
    
    printf("Numeric Computation Calculator\n");
    printf("Available operations:\n");
    for (int i = 0; i < num_operations; i++) {
        printf("%d. %s\n", i + 1, operation_names[i]);
    }
    
    int choice;
    while (1) {
        printf("Select operation (1-%d): ", num_operations);
        char buffer[10];
        if (fgets(buffer, sizeof(buffer), stdin) == NULL) {
            printf("Error reading input\n");
            return 1;
        }
        
        char* endptr;
        choice = strtol(buffer, &endptr, 10);
        
        if (endptr != buffer && *endptr == '\n' && choice >= 1 && choice <= num_operations) {
            break;
        }
        
        printf("Invalid choice. Please enter a number between 1 and %d.\n", num_operations);
    }
    
    double a = safe_input("Enter first number: ");
    double b = safe_input("Enter second number: ");
    
    numeric_operation selected_op = operations[choice - 1];
    double result = selected_op(a, b);
    
    printf("Result of %s: %.6f\n", operation_names[choice - 1], result);
    
    return 0;
}