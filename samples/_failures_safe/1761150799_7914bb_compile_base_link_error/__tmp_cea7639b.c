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
        if (isinf(result)) return INFINITY;
    }
    return result;
}

int main(void) {
    enum Operation op;
    int choice;
    double a, b, result;
    int int_val;
    
    printf("Available operations:\n");
    printf("0: Add\n");
    printf("1: Subtract\n");
    printf("2: Multiply\n");
    printf("3: Divide\n");
    printf("4: Power\n");
    printf("5: Square Root\n");
    printf("6: Absolute Value\n");
    printf("7: Factorial\n");
    printf("Select operation (0-7): ");
    
    if (scanf("%d", &choice) != 1) {
        printf("Error: Invalid input\n");
        return 1;
    }
    
    if (choice < 0 || choice > 7) {
        printf("Error: Invalid operation\n");
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
                printf("Error: Invalid input\n");
                return 1;
            }
            printf("Enter second number: ");
            if (scanf("%lf", &b) != 1) {
                printf("Error: Invalid input\n");
                return 1;
            }
            break;
            
        case SQUARE_ROOT:
        case ABSOLUTE:
            printf("Enter number: ");
            if (scanf("%lf", &a) != 1) {
                printf("Error: Invalid input\n");
                return 1;
            }
            break;
            
        case FACTORIAL:
            printf("Enter integer (0-170): ");
            if (scanf("%d", &int_val) != 1) {
                printf("Error: Invalid input\n");
                return 1;
            }
            if (int_val < 0 || int_val > 170) {
                printf("Error: Value out of range\n");
                return 1;
            }
            break;
    }
    
    switch (op) {
        case ADD:
            result = a + b;
            printf("Result: %.6f\n", result);
            break;
            
        case SUBTRACT:
            result = a - b;
            printf("Result: %.6f\n", result);
            break;
            
        case MULTIPLY:
            result = a * b;
            printf("Result: %.6f\n", result);
            break;
            
        case DIVIDE:
            if (fabs(b) < 1e-15) {
                printf("Error: Division by zero\n");
                return 1;
            }
            result = a / b;
            printf("Result: %.6f\n", result);
            break;
            
        case POWER:
            if (a < 0 && fmod(b, 1.0) != 0) {
                printf("Error: Negative base with fractional exponent\n");
                return 1;
            }
            result = pow(a, b);
            if (isnan(result) || isinf(result)) {
                printf("Error: Invalid power operation\n");
                return 1;
            }
            printf("Result: %.6f\n", result);
            break;
            
        case SQUARE_ROOT:
            if (a < 0) {
                printf("Error: Cannot compute square root of negative number\n");
                return 1;
            }
            result = sqrt(a);
            printf("Result: %.6f\n", result);
            break;
            
        case ABSOLUTE:
            result = fabs(a);
            printf("Result: %.6f\n", result);
            break;
            
        case FACTORIAL:
            result = compute_factorial(int_val);
            if (isnan(result)) {
                printf("Error: Invalid factorial input\n");
                return 1;
            }
            if (isinf(result)) {
                printf("Error: Factorial too large\n");
                return 1;
            }
            printf("Result: %.0f\n", result);
            break;
    }
    
    return 0;
}