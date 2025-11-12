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
    printf("%s", prompt);
    if (fgets(buffer, sizeof(buffer), stdin) == NULL) {
        return 0;
    }
    char* endptr;
    *value = strtod(buffer, &endptr);
    if (endptr == buffer || *endptr != '\n') {
        return 0;
    }
    return 1;
}

int main() {
    numeric_operation operations[] = {add, subtract, multiply, divide, power};
    const char* operation_names[] = {"addition", "subtraction", "multiplication", "division", "exponentiation"};
    const int num_operations = sizeof(operations) / sizeof(operations[0]);
    
    double a, b;
    int choice;
    
    printf("Numeric Computation Calculator\n");
    printf("Available operations:\n");
    for (int i = 0; i < num_operations; i++) {
        printf("%d. %s\n", i + 1, operation_names[i]);
    }
    
    printf("Select operation (1-%d): ", num_operations);
    if (scanf("%d", &choice) != 1) {
        printf("Invalid input.\n");
        return 1;
    }
    while (getchar() != '\n');
    
    if (choice < 1 || choice > num_operations) {
        printf("Invalid operation choice.\n");
        return 1;
    }
    
    if (!get_valid_double("Enter first number: ", &a)) {
        printf("Invalid first number.\n");
        return 1;
    }
    
    if (!get_valid_double("Enter second number: ", &b)) {
        printf("Invalid second number.\n");
        return 1;
    }
    
    numeric_operation op = operations[choice - 1];
    double result = op(a, b);
    
    if (isnan(result)) {
        printf("Error: Invalid operation for given inputs.\n");
        return 1;
    }
    
    printf("Result of %s: %.6f\n", operation_names[choice - 1], result);
    
    return 0;
}