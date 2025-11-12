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

double safe_input(const char* prompt) {
    char buffer[256];
    double value;
    int valid = 0;
    
    while (!valid) {
        printf("%s", prompt);
        if (fgets(buffer, sizeof(buffer), stdin) == NULL) {
            printf("Input error\n");
            exit(1);
        }
        
        char* endptr;
        value = strtod(buffer, &endptr);
        if (endptr != buffer && *endptr == '\n') {
            valid = 1;
        } else {
            printf("Invalid input. Please enter a valid number.\n");
        }
    }
    return value;
}

void perform_operation(numeric_func func, const char* op_name, double a, double b) {
    double result = func(a, b);
    printf("%s(%.2f, %.2f) = ", op_name, a, b);
    
    if (isnan(result)) {
        printf("undefined\n");
    } else if (isinf(result)) {
        printf("infinity\n");
    } else {
        printf("%.6f\n", result);
    }
}

int main() {
    numeric_func operations[] = {add, subtract, multiply, divide, power};
    const char* operation_names[] = {"add", "subtract", "multiply", "divide", "power"};
    const int num_operations = sizeof(operations) / sizeof(operations[0]);
    
    printf("Numeric Computation with Function Pointers\n");
    printf("Available operations: add, subtract, multiply, divide, power\n\n");
    
    double a = safe_input("Enter first number: ");
    double b = safe_input("Enter second number: ");
    
    printf("\nResults:\n");
    for (int i = 0; i < num_operations; i++) {
        perform_operation(operations[i], operation_names[i], a, b);
    }
    
    printf("\nComputing mathematical series using function pointers...\n");
    
    double sum = 0.0;
    int terms = (int)safe_input("Enter number of terms for series (1-1000): ");
    if (terms < 1 || terms > 1000) {
        printf("Invalid number of terms. Using default 10.\n");
        terms = 10;
    }
    
    for (int i = 1; i <= terms; i++) {
        double term_value = operations[i % num_operations]((double)i, 2.0);
        if (!isnan(term_value) && !isinf(term_value)) {
            sum += term_value;
        }
    }
    
    printf("Series sum with %d terms: %.6f\n", terms, sum);
    
    return 0;
}