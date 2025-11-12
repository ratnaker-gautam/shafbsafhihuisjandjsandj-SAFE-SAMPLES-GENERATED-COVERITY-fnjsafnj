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
    INVALID
};

int main() {
    double num1, num2, result;
    int choice;
    enum Operation op;
    
    printf("Enter first number: ");
    if (scanf("%lf", &num1) != 1) {
        printf("Invalid input for first number\n");
        return 1;
    }
    
    printf("Available operations:\n");
    printf("0: Addition\n");
    printf("1: Subtraction\n");
    printf("2: Multiplication\n");
    printf("3: Division\n");
    printf("4: Power\n");
    printf("5: Square Root\n");
    printf("Select operation (0-5): ");
    
    if (scanf("%d", &choice) != 1) {
        printf("Invalid operation selection\n");
        return 1;
    }
    
    if (choice < 0 || choice > 5) {
        printf("Operation out of range\n");
        return 1;
    }
    
    op = (enum Operation)choice;
    
    switch (op) {
        case ADD:
            printf("Enter second number: ");
            if (scanf("%lf", &num2) != 1) {
                printf("Invalid input for second number\n");
                return 1;
            }
            result = num1 + num2;
            printf("Result: %.6f\n", result);
            break;
            
        case SUBTRACT:
            printf("Enter second number: ");
            if (scanf("%lf", &num2) != 1) {
                printf("Invalid input for second number\n");
                return 1;
            }
            result = num1 - num2;
            printf("Result: %.6f\n", result);
            break;
            
        case MULTIPLY:
            printf("Enter second number: ");
            if (scanf("%lf", &num2) != 1) {
                printf("Invalid input for second number\n");
                return 1;
            }
            result = num1 * num2;
            printf("Result: %.6f\n", result);
            break;
            
        case DIVIDE:
            printf("Enter second number: ");
            if (scanf("%lf", &num2) != 1) {
                printf("Invalid input for second number\n");
                return 1;
            }
            if (fabs(num2) < 1e-15) {
                printf("Error: Division by zero\n");
                return 1;
            }
            result = num1 / num2;
            printf("Result: %.6f\n", result);
            break;
            
        case POWER:
            printf("Enter exponent: ");
            if (scanf("%lf", &num2) != 1) {
                printf("Invalid input for exponent\n");
                return 1;
            }
            if (num1 < 0 && fmod(num2, 1.0) != 0.0) {
                printf("Error: Negative base with fractional exponent\n");
                return 1;
            }
            result = pow(num1, num2);
            printf("Result: %.6f\n", result);
            break;
            
        case SQUARE_ROOT:
            if (num1 < 0) {
                printf("Error: Cannot calculate square root of negative number\n");
                return 1;
            }
            result = sqrt(num1);
            printf("Result: %.6f\n", result);
            break;
            
        default:
            printf("Invalid operation\n");
            return 1;
    }
    
    return 0;
}