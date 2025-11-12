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

void print_menu(void) {
    printf("Available operations:\n");
    printf("1. Addition\n");
    printf("2. Subtraction\n");
    printf("3. Multiplication\n");
    printf("4. Division\n");
    printf("5. Exponentiation\n");
    printf("Enter operation number (1-5): ");
}

int get_valid_choice(void) {
    int choice;
    char buffer[100];
    
    if (fgets(buffer, sizeof(buffer), stdin) == NULL) {
        fprintf(stderr, "Error reading input\n");
        exit(EXIT_FAILURE);
    }
    
    if (sscanf(buffer, "%d", &choice) != 1) {
        fprintf(stderr, "Invalid input format\n");
        exit(EXIT_FAILURE);
    }
    
    if (choice < 1 || choice > 5) {
        fprintf(stderr, "Choice must be between 1 and 5\n");
        exit(EXIT_FAILURE);
    }
    
    return choice;
}

double get_valid_number(const char* prompt) {
    double value;
    char buffer[100];
    
    printf("%s", prompt);
    
    if (fgets(buffer, sizeof(buffer), stdin) == NULL) {
        fprintf(stderr, "Error reading input\n");
        exit(EXIT_FAILURE);
    }
    
    if (sscanf(buffer, "%lf", &value) != 1) {
        fprintf(stderr, "Invalid number format\n");
        exit(EXIT_FAILURE);
    }
    
    return value;
}

int main(void) {
    numeric_operation operations[] = {add, subtract, multiply, divide, power};
    const char* operation_names[] = {"Addition", "Subtraction", "Multiplication", "Division", "Exponentiation"};
    
    print_menu();
    int choice = get_valid_choice();
    
    double a = get_valid_number("Enter first number: ");
    double b = get_valid_number("Enter second number: ");
    
    numeric_operation op = operations[choice - 1];
    double result = op(a, b);
    
    printf("Result of %s: %.6f\n", operation_names[choice - 1], result);
    
    return EXIT_SUCCESS;
}