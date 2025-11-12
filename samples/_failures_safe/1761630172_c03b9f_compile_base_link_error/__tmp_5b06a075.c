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

void print_menu(void) {
    printf("Available operations:\n");
    printf("1. Addition\n");
    printf("2. Subtraction\n");
    printf("3. Multiplication\n");
    printf("4. Division\n");
    printf("5. Exponentiation\n");
    printf("6. Exit\n");
}

int get_choice(void) {
    int choice;
    char buffer[100];
    
    printf("Enter your choice (1-6): ");
    if (fgets(buffer, sizeof(buffer), stdin) == NULL) {
        return -1;
    }
    
    if (sscanf(buffer, "%d", &choice) != 1) {
        return -1;
    }
    
    return choice;
}

int get_numbers(double *a, double *b) {
    char buffer[100];
    
    printf("Enter first number: ");
    if (fgets(buffer, sizeof(buffer), stdin) == NULL) {
        return 0;
    }
    if (sscanf(buffer, "%lf", a) != 1) {
        return 0;
    }
    
    printf("Enter second number: ");
    if (fgets(buffer, sizeof(buffer), stdin) == NULL) {
        return 0;
    }
    if (sscanf(buffer, "%lf", b) != 1) {
        return 0;
    }
    
    return 1;
}

int main(void) {
    numeric_operation operations[] = {add, subtract, multiply, divide, power};
    int choice;
    double a, b, result;
    
    printf("Numeric Computation Calculator\n");
    printf("==============================\n");
    
    while (1) {
        print_menu();
        choice = get_choice();
        
        if (choice == 6) {
            printf("Goodbye!\n");
            break;
        }
        
        if (choice < 1 || choice > 5) {
            printf("Invalid choice. Please try again.\n");
            continue;
        }
        
        if (!get_numbers(&a, &b)) {
            printf("Invalid input. Please enter valid numbers.\n");
            continue;
        }
        
        result = operations[choice - 1](a, b);
        
        if (isnan(result)) {
            printf("Error: Invalid operation for given numbers.\n");
        } else {
            printf("Result: %.6f\n", result);
        }
        
        printf("\n");
    }
    
    return 0;
}