//DeepSeek-V3 SAFE v1.4 Category: Safe ; Style: function_pointers ; Variation: numeric_computation
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

double safe_input(const char* prompt) {
    char buffer[256];
    double value;
    
    while (1) {
        printf("%s", prompt);
        if (fgets(buffer, sizeof(buffer), stdin) == NULL) {
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

void perform_operation(numeric_func func, const char* op_name, double a, double b) {
    double result = func(a, b);
    
    if (isnan(result)) {
        printf("Operation '%s' failed: invalid operands\n", op_name);
    } else if (isinf(result)) {
        printf("Operation '%s' resulted in infinity\n", op_name);
    } else {
        printf("%s result: %.6f\n", op_name, result);
    }
}

int main() {
    numeric_func operations[] = {add, subtract, multiply, divide, power};
    const char* operation_names[] = {"Addition", "Subtraction", "Multiplication", "Division", "Power"};
    const int num_operations = sizeof(operations) / sizeof(operations[0]);
    
    printf("Numeric Computation Calculator\n");
    printf("Available operations:\n");
    for (int i = 0; i < num_operations; i++) {
        printf("%d. %s\n", i + 1, operation_names[i]);
    }
    
    double a = safe_input("Enter first number: ");
    double b = safe_input("Enter second number: ");
    
    printf("\nPerforming all operations:\n");
    for (int i = 0; i < num_operations; i++) {
        perform_operation(operations[i], operation_names[i], a, b);
    }
    
    printf("\nComputing mathematical series:\n");
    double sum = 0.0;
    for (int i = 1; i <= 10; i++) {
        double term = operations[3]((double)i, operations[1]((double)(i + 1), 1.0));
        if (!isnan(term) && !isinf(term)) {
            sum += term;
        }
    }
    printf("Series sum: %.6f\n", sum);
    
    printf("\nFunction pointer demonstration:\n");
    numeric_func current_func = operations[2];
    for (int i = 1; i <= 5; i++) {
        double x = (double)i * 0.5;
        double y = (double)i * 0.25;
        double res = current_func(x, y);
        if (!isnan(res) && !isinf(res)) {
            printf("f(%.2f, %.2f) = %.6f\n", x, y, res);
        }
    }
    
    return 0;
}