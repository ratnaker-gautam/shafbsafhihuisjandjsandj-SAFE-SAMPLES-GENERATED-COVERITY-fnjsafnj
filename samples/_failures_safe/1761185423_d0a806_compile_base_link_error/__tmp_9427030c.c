//DeepSeek-V3 V2.5 Category: Safe ; Style: function_pointers ; Variation: numeric_computation
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
    return pow(a, b);
}

int get_valid_int(const char* prompt, int min_val, int max_val) {
    char buffer[256];
    long result;
    char* endptr;
    
    while (1) {
        printf("%s", prompt);
        if (fgets(buffer, sizeof(buffer), stdin) == NULL) {
            fprintf(stderr, "Error reading input\n");
            exit(EXIT_FAILURE);
        }
        
        result = strtol(buffer, &endptr, 10);
        if (endptr == buffer || *endptr != '\n') {
            printf("Invalid input. Please enter an integer.\n");
            continue;
        }
        if (result < min_val || result > max_val) {
            printf("Input out of range. Please enter between %d and %d.\n", min_val, max_val);
            continue;
        }
        if (result == LONG_MAX || result == LONG_MIN) {
            printf("Input out of range.\n");
            continue;
        }
        break;
    }
    return (int)result;
}

double get_valid_double(const char* prompt) {
    char buffer[256];
    double result;
    char* endptr;
    
    while (1) {
        printf("%s", prompt);
        if (fgets(buffer, sizeof(buffer), stdin) == NULL) {
            fprintf(stderr, "Error reading input\n");
            exit(EXIT_FAILURE);
        }
        
        result = strtod(buffer, &endptr);
        if (endptr == buffer || *endptr != '\n') {
            printf("Invalid input. Please enter a number.\n");
            continue;
        }
        if (result == HUGE_VAL || result == -HUGE_VAL) {
            printf("Input out of range.\n");
            continue;
        }
        break;
    }
    return result;
}

int main(void) {
    numeric_operation operations[] = {add, subtract, multiply, divide, power};
    const char* operation_names[] = {"Addition", "Subtraction", "Multiplication", "Division", "Power"};
    const int num_operations = sizeof(operations) / sizeof(operations[0]);
    
    printf("Numeric Computation Calculator\n");
    printf("Available operations:\n");
    for (int i = 0; i < num_operations; i++) {
        printf("%d. %s\n", i + 1, operation_names[i]);
    }
    
    int choice = get_valid_int("Select operation (1-5): ", 1, num_operations);
    double a = get_valid_double("Enter first number: ");
    double b = get_valid_double("Enter second number: ");
    
    numeric_operation selected_op = operations[choice - 1];
    double result = selected_op(a, b);
    
    if (!isfinite(result)) {
        fprintf(stderr, "Error: Result is not finite\n");
        return EXIT_FAILURE;
    }
    
    printf("Result: %.6f\n", result);
    
    return EXIT_SUCCESS;
}