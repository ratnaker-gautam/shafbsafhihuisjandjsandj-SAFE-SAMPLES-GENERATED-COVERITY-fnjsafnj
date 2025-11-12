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

void print_menu() {
    printf("Available operations:\n");
    printf("1. Addition\n");
    printf("2. Subtraction\n");
    printf("3. Multiplication\n");
    printf("4. Division\n");
    printf("5. Exponentiation\n");
    printf("Enter choice (1-5): ");
}

int get_valid_choice() {
    int choice;
    char buffer[100];
    
    if (fgets(buffer, sizeof(buffer), stdin) == NULL) {
        return -1;
    }
    
    if (sscanf(buffer, "%d", &choice) != 1) {
        return -1;
    }
    
    if (choice < 1 || choice > 5) {
        return -1;
    }
    
    return choice;
}

int get_valid_double(double *value) {
    char buffer[100];
    
    if (fgets(buffer, sizeof(buffer), stdin) == NULL) {
        return 0;
    }
    
    if (sscanf(buffer, "%lf", value) != 1) {
        return 0;
    }
    
    return 1;
}

int main() {
    numeric_operation operations[] = {add, subtract, multiply, divide, power};
    const char* operation_names[] = {"Addition", "Subtraction", "Multiplication", "Division", "Exponentiation"};
    
    int choice;
    double a, b, result;
    
    printf("Numeric Computation Calculator\n");
    printf("==============================\n\n");
    
    while (1) {
        print_menu();
        choice = get_valid_choice();
        
        if (choice == -1) {
            printf("Invalid choice. Please enter a number between 1 and 5.\n\n");
            continue;
        }
        
        printf("Enter first number: ");
        if (!get_valid_double(&a)) {
            printf("Invalid number.\n\n");
            continue;
        }
        
        printf("Enter second number: ");
        if (!get_valid_double(&b)) {
            printf("Invalid number.\n\n");
            continue;
        }
        
        result = operations[choice - 1](a, b);
        
        if (isnan(result)) {
            printf("Error: Invalid operation for given inputs.\n\n");
        } else {
            printf("%s result: %.6f\n\n", operation_names[choice - 1], result);
        }
        
        printf("Perform another calculation? (y/n): ");
        char response[10];
        if (fgets(response, sizeof(response), stdin) == NULL) {
            break;
        }
        if (response[0] != 'y' && response[0] != 'Y') {
            break;
        }
        printf("\n");
    }
    
    printf("Goodbye!\n");
    return 0;
}