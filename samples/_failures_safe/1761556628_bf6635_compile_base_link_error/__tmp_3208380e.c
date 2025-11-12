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

void perform_operation(numeric_operation op, const char* op_name, double a, double b) {
    double result = op(a, b);
    printf("%.6f %s %.6f = %.6f\n", a, op_name, b, result);
}

int main() {
    numeric_operation operations[] = {add, subtract, multiply, divide, power};
    const char* operation_names[] = {"+", "-", "*", "/", "^"};
    const int num_operations = sizeof(operations) / sizeof(operations[0]);
    
    printf("Numeric Computation with Function Pointers\n");
    printf("Available operations: ");
    for (int i = 0; i < num_operations; i++) {
        printf("%s ", operation_names[i]);
    }
    printf("\n");
    
    double a = safe_input("Enter first number: ");
    double b = safe_input("Enter second number: ");
    
    printf("\nPerforming all operations:\n");
    for (int i = 0; i < num_operations; i++) {
        perform_operation(operations[i], operation_names[i], a, b);
    }
    
    printf("\nComputing series using function pointers:\n");
    double sum = 0.0;
    for (int i = 1; i <= 10; i++) {
        double term = operations[i % num_operations]((double)i, 2.0);
        printf("Term %d: %.6f\n", i, term);
        sum += term;
    }
    printf("Series sum: %.6f\n", sum);
    
    return 0;
}