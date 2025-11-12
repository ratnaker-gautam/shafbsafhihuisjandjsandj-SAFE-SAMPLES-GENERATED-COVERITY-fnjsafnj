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

void print_result(double result) {
    if (isnan(result)) {
        printf("Error: Invalid operation\n");
    } else {
        printf("Result: %.6f\n", result);
    }
}

int main() {
    numeric_operation operations[] = {add, subtract, multiply, divide, power};
    const char* operation_names[] = {"addition", "subtraction", "multiplication", "division", "exponentiation"};
    int num_operations = sizeof(operations) / sizeof(operations[0]);
    
    double a, b;
    int choice;
    
    printf("Numeric Computation Tool\n");
    printf("Available operations:\n");
    for (int i = 0; i < num_operations; i++) {
        printf("%d. %s\n", i + 1, operation_names[i]);
    }
    
    printf("Enter first number: ");
    if (scanf("%lf", &a) != 1) {
        printf("Error: Invalid input for first number\n");
        return 1;
    }
    
    printf("Enter second number: ");
    if (scanf("%lf", &b) != 1) {
        printf("Error: Invalid input for second number\n");
        return 1;
    }
    
    printf("Select operation (1-%d): ", num_operations);
    if (scanf("%d", &choice) != 1) {
        printf("Error: Invalid operation selection\n");
        return 1;
    }
    
    if (choice < 1 || choice > num_operations) {
        printf("Error: Invalid operation choice\n");
        return 1;
    }
    
    numeric_operation selected_op = operations[choice - 1];
    double result = selected_op(a, b);
    print_result(result);
    
    return 0;
}