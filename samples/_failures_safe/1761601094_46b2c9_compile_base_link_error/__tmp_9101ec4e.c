//DeepSeek-V3 V2.5 Category: Safe ; Style: enum_switch ; Variation: numeric_computation
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

enum Operation {
    ADD = 1,
    SUBTRACT,
    MULTIPLY,
    DIVIDE,
    POWER,
    SQUARE_ROOT,
    FACTORIAL
};

double compute_factorial(int n) {
    if (n < 0) return -1.0;
    double result = 1.0;
    for (int i = 2; i <= n; i++) {
        result *= i;
    }
    return result;
}

int main(void) {
    int choice;
    double a, b, result;
    
    printf("Available operations:\n");
    printf("1. Addition\n");
    printf("2. Subtraction\n");
    printf("3. Multiplication\n");
    printf("4. Division\n");
    printf("5. Power\n");
    printf("6. Square Root\n");
    printf("7. Factorial\n");
    printf("Select operation (1-7): ");
    
    if (scanf("%d", &choice) != 1) {
        printf("Invalid input.\n");
        return 1;
    }
    
    if (choice < ADD || choice > FACTORIAL) {
        printf("Invalid operation choice.\n");
        return 1;
    }
    
    switch (choice) {
        case ADD:
            printf("Enter two numbers: ");
            if (scanf("%lf %lf", &a, &b) != 2) {
                printf("Invalid input.\n");
                return 1;
            }
            result = a + b;
            printf("Result: %.6f\n", result);
            break;
            
        case SUBTRACT:
            printf("Enter two numbers: ");
            if (scanf("%lf %lf", &a, &b) != 2) {
                printf("Invalid input.\n");
                return 1;
            }
            result = a - b;
            printf("Result: %.6f\n", result);
            break;
            
        case MULTIPLY:
            printf("Enter two numbers: ");
            if (scanf("%lf %lf", &a, &b) != 2) {
                printf("Invalid input.\n");
                return 1;
            }
            result = a * b;
            printf("Result: %.6f\n", result);
            break;
            
        case DIVIDE:
            printf("Enter two numbers: ");
            if (scanf("%lf %lf", &a, &b) != 2) {
                printf("Invalid input.\n");
                return 1;
            }
            if (fabs(b) < 1e-12) {
                printf("Error: Division by zero.\n");
                return 1;
            }
            result = a / b;
            printf("Result: %.6f\n", result);
            break;
            
        case POWER:
            printf("Enter base and exponent: ");
            if (scanf("%lf %lf", &a, &b) != 2) {
                printf("Invalid input.\n");
                return 1;
            }
            if (a < 0.0 && fmod(b, 1.0) != 0.0) {
                printf("Error: Negative base with fractional exponent.\n");
                return 1;
            }
            result = pow(a, b);
            printf("Result: %.6f\n", result);
            break;
            
        case SQUARE_ROOT:
            printf("Enter a non-negative number: ");
            if (scanf("%lf", &a) != 1) {
                printf("Invalid input.\n");
                return 1;
            }
            if (a < 0.0) {
                printf("Error: Cannot compute square root of negative number.\n");
                return 1;
            }
            result = sqrt(a);
            printf("Result: %.6f\n", result);
            break;
            
        case FACTORIAL:
            printf("Enter a non-negative integer: ");
            if (scanf("%lf", &a) != 1) {
                printf("Invalid input.\n");
                return 1;
            }
            if (a < 0.0 || fmod(a, 1.0) != 0.0) {
                printf("Error: Input must be a non-negative integer.\n");
                return 1;
            }
            result = compute_factorial((int)a);
            if (result < 0.0) {
                printf("Error: Invalid input for factorial.\n");
                return 1;
            }
            printf("Result: %.0f\n", result);
            break;
            
        default:
            printf("Invalid operation.\n");
            return 1;
    }
    
    return 0;
}