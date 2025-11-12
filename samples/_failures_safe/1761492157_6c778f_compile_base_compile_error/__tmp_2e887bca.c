//DeepSeek-V3 V2.5 Category: Safe ; Style: enum_switch ; Variation: numeric_computation
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <limits.h>

enum Operation {
    ADD,
    SUBTRACT,
    MULTIPLY,
    DIVIDE,
    POWER,
    SQUARE_ROOT,
    FACTORIAL
};

long factorial(int n) {
    if (n < 0) return -1;
    long result = 1;
    for (int i = 1; i <= n; i++) {
        if (result > LONG_MAX / i) return -1;
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
        printf("Invalid input\n");
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
        printf("Invalid input\n");
        return 1;
    }
    
    if (op_choice < 0 || op_choice > 6) {
        printf("Invalid operation\n");
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
                printf("Invalid input\n");
                return 1;
            }
            break;
        default:
            b = 0;
            break;
    }
    
    double result = 0;
    int valid = 1;
    
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
                valid = 0;
            } else {
                result = a / b;
            }
            break;
        case POWER:
            if (a < 0 && fmod(b, 1.0) != 0) {
                printf("Error: Negative base with fractional exponent\n");
                valid = 0;
            } else {
                result = pow(a, b);
            }
            break;
        case SQUARE_ROOT:
            if (a < 0) {
                printf("Error: Square root of negative number\n");
                valid = 0;
            } else {
                result = sqrt(a);
            }
            break;
        case FACTORIAL:
            if (a < 0 || fmod(a, 1.0) != 0) {
                printf("Error: Factorial requires non-negative integer\n");
                valid = 0;
            } else {
                long fact = factorial((int)a);
                if (fact == -1) {
                    printf("Error: Factorial overflow\n");
                    valid = 0;
                } else {
                    result = (double)fact;
                }
            }
            break;
        default:
            valid = 0;
            break;
    }
    
    if (valid) {
        printf("Result: %.6f\n", result);
    }
    
    return 0;
}