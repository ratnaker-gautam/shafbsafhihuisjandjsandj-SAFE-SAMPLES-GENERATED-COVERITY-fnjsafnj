//DeepSeek-V3 SAFE v1.4 Category: Safe ; Style: function_pointers ; Variation: numeric_computation
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
    double result = pow(a, b);
    if (isinf(result) || isnan(result)) {
        fprintf(stderr, "Error: Power operation result out of range\n");
        exit(EXIT_FAILURE);
    }
    return result;
}

void print_menu(void) {
    printf("Numeric Computation Menu:\n");
    printf("1. Addition\n");
    printf("2. Subtraction\n");
    printf("3. Multiplication\n");
    printf("4. Division\n");
    printf("5. Power\n");
    printf("6. Exit\n");
    printf("Enter your choice (1-6): ");
}

int get_valid_int(const char* prompt, int min, int max) {
    char buffer[256];
    long value;
    char* endptr;
    
    while (1) {
        printf("%s", prompt);
        if (fgets(buffer, sizeof(buffer), stdin) == NULL) {
            fprintf(stderr, "Error reading input\n");
            exit(EXIT_FAILURE);
        }
        
        value = strtol(buffer, &endptr, 10);
        if (endptr == buffer || *endptr != '\n') {
            printf("Invalid input. Please enter an integer.\n");
            continue;
        }
        if (value < min || value > max) {
            printf("Input out of range. Please enter between %d and %d.\n", min, max);
            continue;
        }
        if (value == LONG_MIN || value == LONG_MAX) {
            printf("Input out of range.\n");
            continue;
        }
        break;
    }
    return (int)value;
}

double get_valid_double(const char* prompt) {
    char buffer[256];
    double value;
    char* endptr;
    
    while (1) {
        printf("%s", prompt);
        if (fgets(buffer, sizeof(buffer), stdin) == NULL) {
            fprintf(stderr, "Error reading input\n");
            exit(EXIT_FAILURE);
        }
        
        value = strtod(buffer, &endptr);
        if (endptr == buffer || *endptr != '\n') {
            printf("Invalid input. Please enter a number.\n");
            continue;
        }
        if (value == HUGE_VAL || value == -HUGE_VAL) {
            printf("Input out of range.\n");
            continue;
        }
        break;
    }
    return value;
}

int main(void) {
    numeric_operation operations[] = {add, subtract, multiply, divide, power};
    const char* operation_names[] = {"Addition", "Subtraction", "Multiplication", "Division", "Power"};
    int choice;
    double a, b, result;
    
    printf("Numeric Computation Program\n");
    printf("===========================\n");
    
    while (1) {
        print_menu();
        choice = get_valid_int("", 1, 6);
        
        if (choice == 6) {
            printf("Goodbye!\n");
            break;
        }
        
        a = get_valid_double("Enter first number: ");
        b = get_valid_double("Enter second number: ");
        
        if (choice >= 1 && choice <= 5) {
            result = operations[choice - 1](a, b);
            printf("%s: %.6f %c %.6f = %.6f\n", 
                   operation_names[choice - 1], a, 
                   (choice == 1 ? '+' : choice == 2 ? '-' : choice == 3 ? '*' : choice == 4 ? '/' : '^'), 
                   b, result);
        }
        
        printf("\n");
    }
    
    return 0;
}