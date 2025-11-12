//DeepSeek-V3 V2.5 Category: Safe ; Style: function_pointers ; Variation: numeric_computation
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <stdint.h>
#include <limits.h>
#include <float.h>

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
    if (a == 0 && b < 0) {
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
    if (endptr == buffer) {
        return 0;
    }
    
    return 1;
}

int main(void) {
    numeric_func operations[] = {add, subtract, multiply, divide, power};
    const char* operation_names[] = {"Addition", "Subtraction", "Multiplication", "Division", "Power"};
    const int num_operations = sizeof(operations) / sizeof(operations[0]);
    
    double a, b;
    int choice;
    
    printf("Numeric Computation Tool\n");
    printf("Available operations:\n");
    for (int i = 0; i < num_operations; i++) {
        printf("%d. %s\n", i + 1, operation_names[i]);
    }
    
    printf("Select operation (1-%d): ", num_operations);
    if (scanf("%d", &choice) != 1) {
        printf("Invalid input\n");
        return 1;
    }
    getchar();
    
    if (choice < 1 || choice > num_operations) {
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
    
    numeric_func selected_func = operations[choice - 1];
    double result = selected_func(a, b);
    
    if (isnan(result)) {
        printf("Error: Invalid operation for given inputs\n");
        return 1;
    }
    
    if (result > DBL_MAX || result < -DBL_MAX) {
        printf("Error: Result out of representable range\n");
        return 1;
    }
    
    if (result != 0.0 && fabs(result) < DBL_MIN) {
        printf("Error: Result underflow\n");
        return 1;
    }
    
    printf("Result: %.6f\n", result);
    
    return 0;
}