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
        
        char choice_buffer[10];
        if (fgets(choice_buffer, sizeof(choice_buffer), stdin) == NULL) {
            fprintf(stderr, "Error reading input\n");
            return EXIT_FAILURE;
        }
        
        int choice;
        if (sscanf(choice_buffer, "%d", &choice) != 1) {
            printf("Invalid input. Please enter a number between 1 and 6.\n");
            continue;
        }
        
        if (choice == 6) {
            printf("Goodbye!\n");
            break;
        }
        
        if (choice < 1 || choice > num_operations) {
            printf("Invalid choice. Please select between 1 and 6.\n");
            continue;
        }
        
        double a = safe_input("Enter first number: ");
        double b = safe_input("Enter second number: ");
        
        numeric_operation op = operations[choice - 1];
        double result = op(a, b);
        
        printf("%s: %.6f %s %.6f = %.6f\n", 
               operation_names[choice - 1], a, 
               (choice == 1) ? "+" : (choice == 2) ? "-" : 
               (choice == 3) ? "*" : (choice == 4) ? "/" : "^", 
               b, result);
    }
    
    return EXIT_SUCCESS;
}