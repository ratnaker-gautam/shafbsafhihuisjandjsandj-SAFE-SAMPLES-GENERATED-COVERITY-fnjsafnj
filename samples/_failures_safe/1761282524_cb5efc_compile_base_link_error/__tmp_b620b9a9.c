//DeepSeek-V3 V2.5 Category: Safe ; Style: enum_switch ; Variation: numeric_computation
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

enum Operation {
    ADD,
    SUBTRACT,
    MULTIPLY,
    DIVIDE,
    POWER,
    SQUARE_ROOT,
    FACTORIAL
};

int validate_factorial(int n) {
    if (n < 0) return 0;
    if (n > 12) return 0;
    return 1;
}

unsigned long long compute_factorial(int n) {
    unsigned long long result = 1;
    for (int i = 1; i <= n; i++) {
        result *= i;
    }
    return result;
}

int main() {
    enum Operation op;
    double a, b, result;
    int int_val;
    char op_input[20];
    
    printf("Available operations:\n");
    printf("0: ADD, 1: SUBTRACT, 2: MULTIPLY, 3: DIVIDE\n");
    printf("4: POWER, 5: SQUARE_ROOT, 6: FACTORIAL\n");
    printf("Enter operation number: ");
    
    if (scanf("%19s", op_input) != 1) {
        printf("Error reading operation\n");
        return 1;
    }
    
    char *endptr;
    long op_long = strtol(op_input, &endptr, 10);
    if (*endptr != '\0' || op_long < 0 || op_long > 6) {
        printf("Invalid operation selection\n");
        return 1;
    }
    op = (enum Operation)op_long;
    
    switch (op) {
        case ADD:
        case SUBTRACT:
        case MULTIPLY:
        case DIVIDE:
        case POWER:
            printf("Enter first number: ");
            if (scanf("%lf", &a) != 1) {
                printf("Invalid number\n");
                return 1;
            }
            printf("Enter second number: ");
            if (scanf("%lf", &b) != 1) {
                printf("Invalid number\n");
                return 1;
            }
            break;
            
        case SQUARE_ROOT:
        case FACTORIAL:
            printf("Enter number: ");
            if (scanf("%lf", &a) != 1) {
                printf("Invalid number\n");
                return 1;
            }
            break;
    }
    
    switch (op) {
        case ADD:
            result = a + b;
            printf("Result: %.6lf\n", result);
            break;
            
        case SUBTRACT:
            result = a - b;
            printf("Result: %.6lf\n", result);
            break;
            
        case MULTIPLY:
            result = a * b;
            printf("Result: %.6lf\n", result);
            break;
            
        case DIVIDE:
            if (fabs(b) < 1e-12) {
                printf("Error: Division by zero\n");
                return 1;
            }
            result = a / b;
            printf("Result: %.6lf\n", result);
            break;
            
        case POWER:
            if (a < 0 && fmod(b, 1.0) != 0.0) {
                printf("Error: Negative base with fractional exponent\n");
                return 1;
            }
            result = pow(a, b);
            printf("Result: %.6lf\n", result);
            break;
            
        case SQUARE_ROOT:
            if (a < 0) {
                printf("Error: Square root of negative number\n");
                return 1;
            }
            result = sqrt(a);
            printf("Result: %.6lf\n", result);
            break;
            
        case FACTORIAL:
            if (fmod(a, 1.0) != 0.0 || a < 0) {
                printf("Error: Factorial requires non-negative integer\n");
                return 1;
            }
            int_val = (int)a;
            if (!validate_factorial(int_val)) {
                printf("Error: Factorial input out of range (0-12)\n");
                return 1;
            }
            unsigned long long fact_result = compute_factorial(int_val);
            printf("Result: %llu\n", fact_result);
            break;
    }
    
    return 0;
}