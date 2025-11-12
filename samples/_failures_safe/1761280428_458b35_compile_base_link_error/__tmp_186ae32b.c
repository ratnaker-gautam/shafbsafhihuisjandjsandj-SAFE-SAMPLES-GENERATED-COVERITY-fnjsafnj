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

void perform_operation(numeric_func func, const char* op_name) {
    double a = safe_input("Enter first number: ");
    double b = safe_input("Enter second number: ");
    
    double result = func(a, b);
    
    if (isnan(result)) {
        printf("Operation '%s' resulted in an error.\n", op_name);
    } else {
        printf("%s result: %.6f\n", op_name, result);
    }
}

int main() {
    numeric_func operations[] = {add, subtract, multiply, divide, power};
    const char* operation_names[] = {"Addition", "Subtraction", "Multiplication", "Division", "Power"};
    const int num_operations = sizeof(operations) / sizeof(operations[0]);
    
    while (1) {
        printf("\nAvailable operations:\n");
        for (int i = 0; i < num_operations; i++) {
            printf("%d. %s\n", i + 1, operation_names[i]);
        }
        printf("0. Exit\n");
        
        double choice = safe_input("Select operation (0-5): ");
        
        if (choice < 0 || choice > num_operations || fmod(choice, 1.0) != 0) {
            printf("Invalid choice. Please enter 0-5.\n");
            continue;
        }
        
        int int_choice = (int)choice;
        if (int_choice == 0) {
            break;
        }
        
        if (int_choice >= 1 && int_choice <= num_operations) {
            perform_operation(operations[int_choice - 1], operation_names[int_choice - 1]);
        }
    }
    
    printf("Goodbye!\n");
    return 0;
}