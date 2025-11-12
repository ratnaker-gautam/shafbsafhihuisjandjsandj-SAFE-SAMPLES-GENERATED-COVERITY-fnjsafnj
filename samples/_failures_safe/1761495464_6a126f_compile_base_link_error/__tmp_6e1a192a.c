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

double compute_factorial(int n) {
    if (n < 0) return -1.0;
    double result = 1.0;
    for (int i = 2; i <= n; i++) {
        result *= i;
    }
    return result;
}

int main() {
    enum Operation op;
    double a, b, result;
    int choice, int_val;
    
    printf("Available operations:\n");
    printf("0: Add\n1: Subtract\n2: Multiply\n3: Divide\n4: Power\n5: Square Root\n6: Factorial\n");
    printf("Select operation (0-6): ");
    
    if (scanf("%d", &choice) != 1) {
        printf("Invalid input\n");
        return 1;
    }
    
    if (choice < 0 || choice > 6) {
        printf("Invalid operation\n");
        return 1;
    }
    
    op = (enum Operation)choice;
    
    switch (op) {
        case ADD:
            printf("Enter two numbers: ");
            if (scanf("%lf %lf", &a, &b) != 2) {
                printf("Invalid input\n");
                return 1;
            }
            result = a + b;
            printf("Result: %.6f\n", result);
            break;
            
        case SUBTRACT:
            printf("Enter two numbers: ");
            if (scanf("%lf %lf", &a, &b) != 2) {
                printf("Invalid input\n");
                return 1;
            }
            result = a - b;
            printf("Result: %.6f\n", result);
            break;
            
        case MULTIPLY:
            printf("Enter two numbers: ");
            if (scanf("%lf %lf", &a, &b) != 2) {
                printf("Invalid input\n");
                return 1;
            }
            result = a * b;
            printf("Result: %.6f\n", result);
            break;
            
        case DIVIDE:
            printf("Enter two numbers: ");
            if (scanf("%lf %lf", &a, &b) != 2) {
                printf("Invalid input\n");
                return 1;
            }
            if (fabs(b) < 1e-12) {
                printf("Error: Division by zero\n");
                return 1;
            }
            result = a / b;
            printf("Result: %.6f\n", result);
            break;
            
        case POWER:
            printf("Enter base and exponent: ");
            if (scanf("%lf %lf", &a, &b) != 2) {
                printf("Invalid input\n");
                return 1;
            }
            if (a < 0 && fmod(b, 1.0) != 0.0) {
                printf("Error: Negative base with fractional exponent\n");
                return 1;
            }
            result = pow(a, b);
            printf("Result: %.6f\n", result);
            break;
            
        case SQUARE_ROOT:
            printf("Enter a number: ");
            if (scanf("%lf", &a) != 1) {
                printf("Invalid input\n");
                return 1;
            }
            if (a < 0) {
                printf("Error: Square root of negative number\n");
                return 1;
            }
            result = sqrt(a);
            printf("Result: %.6f\n", result);
            break;
            
        case FACTORIAL:
            printf("Enter a non-negative integer: ");
            if (scanf("%d", &int_val) != 1) {
                printf("Invalid input\n");
                return 1;
            }
            result = compute_factorial(int_val);
            if (result < 0) {
                printf("Error: Factorial of negative number\n");
                return 1;
            }
            printf("Result: %.0f\n", result);
            break;
            
        default:
            printf("Invalid operation\n");
            return 1;
    }
    
    return 0;
}