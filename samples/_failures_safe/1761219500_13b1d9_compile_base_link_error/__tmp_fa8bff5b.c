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

int get_valid_choice(void) {
    int choice;
    char buffer[100];
    
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
            return 6;
        }
        
        if (sscanf(buffer, "%d", &choice) == 1) {
            if (choice >= 1 && choice <= 6) {
                return choice;
            }
        }
        printf("Invalid choice. Please enter 1-6.\n");
    }
}

double get_valid_number(const char* prompt) {
    double number;
    char buffer[100];
    
    while (1) {
        printf("%s", prompt);
        
        if (fgets(buffer, sizeof(buffer), stdin) == NULL) {
            return 0.0;
        }
        
        if (sscanf(buffer, "%lf", &number) == 1) {
            return number;
        }
        printf("Invalid number. Please try again.\n");
    }
}

int main(void) {
    numeric_operation operations[] = {add, subtract, multiply, divide, power};
    const char* operation_names[] = {"Addition", "Subtraction", "Multiplication", "Division", "Power"};
    
    printf("Numeric Computation Program\n");
    printf("===========================\n");
    
    while (1) {
        int choice = get_valid_choice();
        
        if (choice == 6) {
            printf("Goodbye!\n");
            break;
        }
        
        double a = get_valid_number("Enter first number: ");
        double b = get_valid_number("Enter second number: ");
        
        numeric_operation op = operations[choice - 1];
        double result = op(a, b);
        
        printf("\nOperation: %s\n", operation_names[choice - 1]);
        printf("Input: %.6g and %.6g\n", a, b);
        
        if (isnan(result)) {
            printf("Result: Undefined (invalid operation)\n");
        } else if (isinf(result)) {
            printf("Result: Infinity\n");
        } else {
            printf("Result: %.10g\n", result);
        }
        printf("\n");
    }
    
    return 0;
}