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
    ABSOLUTE,
    FACTORIAL
};

double compute_factorial(int n) {
    if (n < 0) return NAN;
    double result = 1.0;
    for (int i = 1; i <= n; i++) {
        result *= i;
        if (result > 1e308) return INFINITY;
    }
    return result;
}

int main() {
    enum Operation op;
    double a, b, result;
    int choice;
    
    printf("Available operations:\n");
    printf("0: Add\n");
    printf("1: Subtract\n");
    printf("2: Multiply\n");
    printf("3: Divide\n");
    printf("4: Power\n");
    printf("5: Square Root\n");
    printf("6: Absolute\n");
    printf("7: Factorial\n");
    printf("Select operation (0-7): ");
    
    if (scanf("%d", &choice) != 1) {
        printf("Invalid input\n");
        return 1;
    }
    
    if (choice < 0 || choice > 7) {
        printf("Invalid operation\n");
        return 1;
    }
    
    op = (enum Operation)choice;
    
    switch (op) {
        case ADD:
        case SUBTRACT:
        case MULTIPLY:
        case DIVIDE:
        case POWER:
            printf("Enter first number: ");
            if (scanf("%lf", &a) != 1) {
                printf("Invalid input\n");
                return 1;
            }
            printf("Enter second number: ");
            if (scanf("%lf", &b) != 1) {
                printf("Invalid input\n");
                return 1;
            }
            break;
            
        case SQUARE_ROOT:
        case ABSOLUTE:
            printf("Enter number: ");
            if (scanf("%lf", &a) != 1) {
                printf("Invalid input\n");
                return 1;
            }
            break;
            
        case FACTORIAL:
            printf("Enter integer: ");
            if (scanf("%lf", &a) != 1) {
                printf("Invalid input\n");
                return 1;
            }
            if (a != (int)a) {
                printf("Factorial requires integer\n");
                return 1;
            }
            break;
    }
    
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
            if (b == 0.0) {
                printf("Division by zero\n");
                return 1;
            }
            result = a / b;
            break;
            
        case POWER:
            if (a == 0.0 && b < 0.0) {
                printf("Undefined: 0 to negative power\n");
                return 1;
            }
            result = pow(a, b);
            break;
            
        case SQUARE_ROOT:
            if (a < 0.0) {
                printf("Square root of negative number\n");
                return 1;
            }
            result = sqrt(a);
            break;
            
        case ABSOLUTE:
            result = fabs(a);
            break;
            
        case FACTORIAL:
            result = compute_factorial((int)a);
            break;
    }
    
    if (isnan(result)) {
        printf("Result is not a number\n");
        return 1;
    }
    
    if (isinf(result)) {
        printf("Result is infinite\n");
        return 1;
    }
    
    printf("Result: %.6f\n", result);
    return 0;
}