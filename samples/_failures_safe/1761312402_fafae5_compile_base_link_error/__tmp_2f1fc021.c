//DeepSeek-V3 V2.5 Category: Safe ; Style: function_pointers ; Variation: numeric_computation
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <stdint.h>
#include <limits.h>

typedef double (*numeric_func)(double, double);

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

int validate_double_input(const char* input, double* result) {
    char* endptr;
    *result = strtod(input, &endptr);
    if (endptr == input || *endptr != '\n') {
        return 0;
    }
    return 1;
}

int main() {
    numeric_func operations[] = {add, subtract, multiply, divide, power};
    const char* operation_names[] = {"addition", "subtraction", "multiplication", "division", "exponentiation"};
    const int num_operations = 5;
    
    char input_buffer[256];
    double a, b;
    int choice;
    
    printf("Available operations:\n");
    for (int i = 0; i < num_operations; i++) {
        printf("%d. %s\n", i + 1, operation_names[i]);
    }
    
    printf("Select operation (1-%d): ", num_operations);
    if (fgets(input_buffer, sizeof(input_buffer), stdin) == NULL) {
        fprintf(stderr, "Error reading input\n");
        return 1;
    }
    
    char* endptr;
    choice = strtol(input_buffer, &endptr, 10);
    if (endptr == input_buffer || *endptr != '\n' || choice < 1 || choice > num_operations) {
        fprintf(stderr, "Invalid operation selection\n");
        return 1;
    }
    
    printf("Enter first number: ");
    if (fgets(input_buffer, sizeof(input_buffer), stdin) == NULL) {
        fprintf(stderr, "Error reading input\n");
        return 1;
    }
    if (!validate_double_input(input_buffer, &a)) {
        fprintf(stderr, "Invalid number format\n");
        return 1;
    }
    
    printf("Enter second number: ");
    if (fgets(input_buffer, sizeof(input_buffer), stdin) == NULL) {
        fprintf(stderr, "Error reading input\n");
        return 1;
    }
    if (!validate_double_input(input_buffer, &b)) {
        fprintf(stderr, "Invalid number format\n");
        return 1;
    }
    
    numeric_func selected_op = operations[choice - 1];
    double result = selected_op(a, b);
    
    if (isnan(result)) {
        printf("Error: Invalid operation for given inputs\n");
        return 1;
    }
    
    printf("Result of %s: %.6f\n", operation_names[choice - 1], result);
    
    return 0;
}