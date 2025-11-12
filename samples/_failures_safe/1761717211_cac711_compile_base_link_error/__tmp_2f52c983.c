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

int get_valid_operation(void) {
    int op;
    char buffer[100];
    
    if (fgets(buffer, sizeof(buffer), stdin) == NULL) {
        fprintf(stderr, "Error reading input\n");
        exit(EXIT_FAILURE);
    }
    
    if (sscanf(buffer, "%d", &op) != 1) {
        fprintf(stderr, "Invalid input format\n");
        exit(EXIT_FAILURE);
    }
    
    if (op < 1 || op > 5) {
        fprintf(stderr, "Operation must be between 1 and 5\n");
        exit(EXIT_FAILURE);
    }
    
    return op;
}

double get_valid_number(const char* prompt) {
    double num;
    char buffer[100];
    
    printf("%s", prompt);
    
    if (fgets(buffer, sizeof(buffer), stdin) == NULL) {
        fprintf(stderr, "Error reading input\n");
        exit(EXIT_FAILURE);
    }
    
    if (sscanf(buffer, "%lf", &num) != 1) {
        fprintf(stderr, "Invalid number format\n");
        exit(EXIT_FAILURE);
    }
    
    return num;
}

int main(void) {
    numeric_operation operations[] = {add, subtract, multiply, divide, power};
    const char* operation_names[] = {"Addition", "Subtraction", "Multiplication", "Division", "Exponentiation"};
    
    print_menu();
    int op_choice = get_valid_operation();
    
    double num1 = get_valid_number("Enter first number: ");
    double num2 = get_valid_number("Enter second number: ");
    
    numeric_operation selected_op = operations[op_choice - 1];
    double result = selected_op(num1, num2);
    
    printf("Result of %s: %.6f\n", operation_names[op_choice - 1], result);
    
    return EXIT_SUCCESS;
}