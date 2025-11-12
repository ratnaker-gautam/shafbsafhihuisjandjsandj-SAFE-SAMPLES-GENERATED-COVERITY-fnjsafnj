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
            printf("Input out of range. Please enter a finite number.\n");
            continue;
        }
        
        break;
    }
    
    return value;
}

int safe_menu_input(void) {
    char buffer[256];
    long choice;
    
    while (1) {
        printf("Choose operation:\n");
        printf("1. Add\n");
        printf("2. Subtract\n");
        printf("3. Multiply\n");
        printf("4. Divide\n");
        printf("5. Power\n");
        printf("6. Exit\n");
        printf("Enter choice (1-6): ");
        
        if (fgets(buffer, sizeof(buffer), stdin) == NULL) {
            fprintf(stderr, "Error reading input\n");
            exit(EXIT_FAILURE);
        }
        
        char* endptr;
        choice = strtol(buffer, &endptr, 10);
        
        if (endptr == buffer || *endptr != '\n') {
            printf("Invalid input. Please enter a number.\n");
            continue;
        }
        
        if (choice < 1 || choice > 6) {
            printf("Choice must be between 1 and 6.\n");
            continue;
        }
        
        break;
    }
    
    return (int)choice;
}

int main(void) {
    numeric_operation operations[] = {add, subtract, multiply, divide, power};
    const char* operation_names[] = {"Addition", "Subtraction", "Multiplication", "Division", "Power"};
    
    while (1) {
        int choice = safe_menu_input();
        
        if (choice == 6) {
            printf("Goodbye!\n");
            break;
        }
        
        double a = safe_input("Enter first number: ");
        double b = safe_input("Enter second number: ");
        
        numeric_operation op = operations[choice - 1];
        double result = op(a, b);
        
        printf("%s: %.6f %s %.6f = %.6f\n", 
               operation_names[choice - 1], a, 
               choice == 1 ? "+" : choice == 2 ? "-" : choice == 3 ? "*" : choice == 4 ? "/" : "^", 
               b, result);
        printf("\n");
    }
    
    return 0;
}