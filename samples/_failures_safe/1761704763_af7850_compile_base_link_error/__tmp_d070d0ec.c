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

int get_valid_double(const char* prompt, double* value) {
    char buffer[256];
    char* endptr;
    
    printf("%s", prompt);
    if (fgets(buffer, sizeof(buffer), stdin) == NULL) {
        return 0;
    }
    
    *value = strtod(buffer, &endptr);
    if (endptr == buffer || *endptr != '\n') {
        return 0;
    }
    
    return 1;
}

int main(void) {
    numeric_operation operations[] = {add, subtract, multiply, divide, power};
    const char* operation_names[] = {"Addition", "Subtraction", "Multiplication", "Division", "Exponentiation"};
    const int num_operations = 5;
    
    double a, b;
    int choice;
    char buffer[256];
    char* endptr;
    
    printf("Numeric Computation Calculator\n");
    printf("Available operations:\n");
    for (int i = 0; i < num_operations; i++) {
        printf("%d. %s\n", i + 1, operation_names[i]);
    }
    
    printf("Select operation (1-%d): ", num_operations);
    if (fgets(buffer, sizeof(buffer), stdin) == NULL) {
        printf("Error reading input\n");
        return 1;
    }
    
    choice = strtol(buffer, &endptr, 10);
    if (endptr == buffer || *endptr != '\n' || choice < 1 || choice > num_operations) {
        printf("Invalid operation selection\n");
        return 1;
    }
    
    if (!get_valid_double("Enter first number: ", &a)) {
        printf("Invalid first number\n");
        return 1;
    }
    
    if (!get_valid_double("Enter second number: ", &b)) {
        printf("Invalid second number\n");
        return 1;
    }
    
    numeric_operation op = operations[choice - 1];
    double result = op(a, b);
    
    if (isnan(result)) {
        printf("Error: Invalid operation for given inputs\n");
        return 1;
    }
    
    if (isinf(result)) {
        printf("Error: Result is infinite\n");
        return 1;
    }
    
    printf("Result: %.6f\n", result);
    
    return 0;
}