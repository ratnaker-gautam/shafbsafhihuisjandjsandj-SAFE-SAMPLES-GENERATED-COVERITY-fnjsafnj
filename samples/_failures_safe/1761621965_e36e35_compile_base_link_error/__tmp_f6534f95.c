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

int main() {
    enum Operation op;
    int choice;
    double a, b, result;
    
    printf("Available operations:\n");
    printf("0: Add\n1: Subtract\n2: Multiply\n3: Divide\n4: Power\n5: Square Root\n6: Absolute\n7: Factorial\n");
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
            if (b == 0.0) {
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
            if (a < 0.0 && fmod(b, 1.0) != 0.0) {
                printf("Error: Negative base with fractional exponent\n");
                return 1;
            }
            result = pow(a, b);
            if (isinf(result) || isnan(result)) {
                printf("Error: Invalid power operation\n");
                return 1;
            }
            printf("Result: %.6f\n", result);
            break;
            
        case SQUARE_ROOT:
            printf("Enter a number: ");
            if (scanf("%lf", &a) != 1) {
                printf("Invalid input\n");
                return 1;
            }
            if (a < 0.0) {
                printf("Error: Cannot compute square root of negative number\n");
                return 1;
            }
            result = sqrt(a);
            printf("Result: %.6f\n", result);
            break;
            
        case ABSOLUTE:
            printf("Enter a number: ");
            if (scanf("%lf", &a) != 1) {
                printf("Invalid input\n");
                return 1;
            }
            result = fabs(a);
            printf("Result: %.6f\n", result);
            break;
            
        case FACTORIAL:
            printf("Enter a non-negative integer: ");
            if (scanf("%lf", &a) != 1) {
                printf("Invalid input\n");
                return 1;
            }
            if (a < 0.0 || fmod(a, 1.0) != 0.0) {
                printf("Error: Input must be non-negative integer\n");
                return 1;
            }
            result = compute_factorial((int)a);
            if (isnan(result)) {
                printf("Error: Invalid input for factorial\n");
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