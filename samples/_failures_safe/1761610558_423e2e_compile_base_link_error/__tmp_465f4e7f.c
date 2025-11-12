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
    printf("Enter operation number (1-5): ");
}

int get_valid_int(const char* prompt, int min, int max) {
    int value;
    char buffer[100];
    
    while (1) {
        printf("%s", prompt);
        if (fgets(buffer, sizeof(buffer), stdin) == NULL) {
            continue;
        }
        
        if (sscanf(buffer, "%d", &value) != 1) {
            printf("Invalid input. Please enter a number.\n");
            continue;
        }
        
        if (value < min || value > max) {
            printf("Value must be between %d and %d.\n", min, max);
            continue;
        }
        
        break;
    }
    return value;
}

double get_valid_double(const char* prompt) {
    double value;
    char buffer[100];
    
    while (1) {
        printf("%s", prompt);
        if (fgets(buffer, sizeof(buffer), stdin) == NULL) {
            continue;
        }
        
        if (sscanf(buffer, "%lf", &value) != 1) {
            printf("Invalid input. Please enter a number.\n");
            continue;
        }
        
        break;
    }
    return value;
}

int main() {
    numeric_operation operations[] = {add, subtract, multiply, divide, power};
    const char* operation_names[] = {"Addition", "Subtraction", "Multiplication", "Division", "Exponentiation"};
    
    while (1) {
        print_menu();
        int choice = get_valid_int("", 1, 5);
        
        printf("Enter first number: ");
        double a = get_valid_double("");
        
        printf("Enter second number: ");
        double b = get_valid_double("");
        
        numeric_operation op = operations[choice - 1];
        double result = op(a, b);
        
        if (isnan(result)) {
            printf("Error: Invalid operation for given inputs.\n");
        } else {
            printf("Result of %s: %.6f\n", operation_names[choice - 1], result);
        }
        
        printf("Perform another calculation? (1 for yes, 0 for no): ");
        int continue_calc = get_valid_int("", 0, 1);
        if (!continue_calc) {
            break;
        }
    }
    
    return 0;
}