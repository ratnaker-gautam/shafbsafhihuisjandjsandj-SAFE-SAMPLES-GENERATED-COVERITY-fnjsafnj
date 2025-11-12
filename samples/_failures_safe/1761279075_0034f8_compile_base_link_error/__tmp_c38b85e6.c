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

void display_menu(void) {
    printf("\nNumeric Computation Menu:\n");
    printf("1. Addition\n");
    printf("2. Subtraction\n");
    printf("3. Multiplication\n");
    printf("4. Division\n");
    printf("5. Exponentiation\n");
    printf("6. Exit\n");
    printf("Enter your choice (1-6): ");
}

int main(void) {
    numeric_operation operations[] = {add, subtract, multiply, divide, power};
    const char* operation_names[] = {"Addition", "Subtraction", "Multiplication", "Division", "Exponentiation"};
    const int num_operations = 5;
    
    while (1) {
        display_menu();
        
        char choice_str[10];
        if (fgets(choice_str, sizeof(choice_str), stdin) == NULL) {
            fprintf(stderr, "Error reading choice\n");
            return EXIT_FAILURE;
        }
        
        char* endptr;
        long choice = strtol(choice_str, &endptr, 10);
        
        if (endptr == choice_str || *endptr != '\n') {
            printf("Invalid input. Please enter a number between 1 and 6.\n");
            continue;
        }
        
        if (choice == 6) {
            printf("Goodbye!\n");
            break;
        }
        
        if (choice < 1 || choice > 5) {
            printf("Invalid choice. Please select between 1 and 6.\n");
            continue;
        }
        
        double a = safe_input("Enter first number: ");
        double b = safe_input("Enter second number: ");
        
        int op_index = choice - 1;
        double result = operations[op_index](a, b);
        
        printf("%s: %.6f %c %.6f = %.6f\n", 
               operation_names[op_index], a, 
               (op_index == 0) ? '+' : (op_index == 1) ? '-' : 
               (op_index == 2) ? '*' : (op_index == 3) ? '/' : '^', 
               b, result);
    }
    
    return EXIT_SUCCESS;
}