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
    if (n < 0 || n > 12) {
        return 0;
    }
    return 1;
}

long long compute_factorial(int n) {
    long long result = 1;
    for (int i = 2; i <= n; i++) {
        result *= i;
    }
    return result;
}

int main() {
    double a, b;
    int op_choice;
    enum Operation op;
    
    printf("Enter first number: ");
    if (scanf("%lf", &a) != 1) {
        printf("Invalid input for first number\n");
        return 1;
    }
    
    printf("Available operations:\n");
    printf("0: Add\n");
    printf("1: Subtract\n");
    printf("2: Multiply\n");
    printf("3: Divide\n");
    printf("4: Power\n");
    printf("5: Square Root\n");
    printf("6: Factorial\n");
    printf("Select operation (0-6): ");
    
    if (scanf("%d", &op_choice) != 1) {
        printf("Invalid operation selection\n");
        return 1;
    }
    
    if (op_choice < 0 || op_choice > 6) {
        printf("Operation out of range\n");
        return 1;
    }
    
    op = (enum Operation)op_choice;
    
    switch (op) {
        case ADD:
        case SUBTRACT:
        case MULTIPLY:
        case DIVIDE:
        case POWER:
            printf("Enter second number: ");
            if (scanf("%lf", &b) != 1) {
                printf("Invalid input for second number\n");
                return 1;
            }
            break;
        case SQUARE_ROOT:
        case FACTORIAL:
            break;
    }
    
    double result = 0.0;
    int valid_operation = 1;
    
    switch (op) {
        case ADD:
            result = a + b;
            break;
        case SUBTRACT:
            result = a - b;
            break;
        case MULTIPLY:
            result = a * b;
            break;
        case DIVIDE:
            if (fabs(b) < 1e-12) {
                printf("Error: Division by zero\n");
                valid_operation = 0;
            } else {
                result = a / b;
            }
            break;
        case POWER:
            if (a < 0 && fmod(b, 1.0) != 0.0) {
                printf("Error: Negative base with fractional exponent\n");
                valid_operation = 0;
            } else {
                result = pow(a, b);
            }
            break;
        case SQUARE_ROOT:
            if (a < 0) {
                printf("Error: Square root of negative number\n");
                valid_operation = 0;
            } else {
                result = sqrt(a);
            }
            break;
        case FACTORIAL:
            if (a != floor(a)) {
                printf("Error: Factorial requires integer\n");
                valid_operation = 0;
            } else {
                int n = (int)a;
                if (!validate_factorial(n)) {
                    printf("Error: Factorial input must be 0-12\n");
                    valid_operation = 0;
                } else {
                    result = (double)compute_factorial(n);
                }
            }
            break;
    }
    
    if (valid_operation) {
        printf("Result: %.6f\n", result);
    }
    
    return 0;
}