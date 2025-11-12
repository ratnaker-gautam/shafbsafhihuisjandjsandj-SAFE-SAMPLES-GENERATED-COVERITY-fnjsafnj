//DeepSeek-V3 V2.5 Category: Safe ; Style: function_pointers ; Variation: numeric_computation
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
        if (endptr != buffer && *endptr == '\n') {
            break;
        }
        printf("Invalid input. Please enter a valid number.\n");
    }
    return value;
}

void perform_operation(numeric_operation op, const char* op_name) {
    double a = safe_input("Enter first number: ");
    double b = safe_input("Enter second number: ");
    
    double result = op(a, b);
    printf("%s result: %.6f\n", op_name, result);
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
    
    while (1) {
        printf("\nSelect operation (1-%d) or 0 to exit: ", num_operations);
        
        char input[256];
        if (fgets(input, sizeof(input), stdin) == NULL) {
            fprintf(stderr, "Error reading input\n");
            return EXIT_FAILURE;
        }
        
        char* endptr;
        long choice = strtol(input, &endptr, 10);
        
        if (endptr == input || *endptr != '\n') {
            printf("Invalid input. Please enter a number.\n");
            continue;
        }
        
        if (choice == 0) {
            printf("Goodbye!\n");
            break;
        }
        
        if (choice < 1 || choice > num_operations) {
            printf("Invalid choice. Please select between 1 and %d.\n", num_operations);
            continue;
        }
        
        int index = (int)(choice - 1);
        if (index >= 0 && index < num_operations) {
            perform_operation(operations[index], operation_names[index]);
        }
    }
    
    return EXIT_SUCCESS;
}