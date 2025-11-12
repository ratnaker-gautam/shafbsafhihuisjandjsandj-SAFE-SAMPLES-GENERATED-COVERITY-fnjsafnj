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

int get_operation_choice() {
    int choice;
    char buffer[256];
    
    while (1) {
        printf("\nSelect operation:\n");
        printf("1. Addition\n");
        printf("2. Subtraction\n");
        printf("3. Multiplication\n");
        printf("4. Division\n");
        printf("5. Exponentiation\n");
        printf("6. Exit\n");
        printf("Enter choice (1-6): ");
        
        if (fgets(buffer, sizeof(buffer), stdin) == NULL) {
            fprintf(stderr, "Error reading input\n");
            exit(EXIT_FAILURE);
        }
        
        if (sscanf(buffer, "%d", &choice) != 1) {
            printf("Invalid input. Please enter a number.\n");
            continue;
        }
        
        if (choice < 1 || choice > 6) {
            printf("Choice must be between 1 and 6.\n");
            continue;
        }
        
        break;
    }
    
    return choice;
}

int main() {
    numeric_operation operations[] = {add, subtract, multiply, divide, power};
    const char* operation_names[] = {"Addition", "Subtraction", "Multiplication", "Division", "Exponentiation"};
    
    printf("Numeric Computation Calculator\n");
    printf("==============================\n");
    
    while (1) {
        int choice = get_operation_choice();
        
        if (choice == 6) {
            printf("Goodbye!\n");
            break;
        }
        
        double a = safe_input("Enter first number: ");
        double b = safe_input("Enter second number: ");
        
        numeric_operation op = operations[choice - 1];
        double result = op(a, b);
        
        printf("\n%s: %.6f %c %.6f = %.6f\n", 
               operation_names[choice - 1], a, 
               choice == 1 ? '+' : choice == 2 ? '-' : choice == 3 ? '*' : choice == 4 ? '/' : '^', 
               b, result);
    }
    
    return 0;
}