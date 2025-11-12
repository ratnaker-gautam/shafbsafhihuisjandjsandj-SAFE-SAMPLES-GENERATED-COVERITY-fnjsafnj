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
    FACTORIAL,
    INVALID
};

int validate_factorial_input(int n) {
    return (n >= 0 && n <= 12);
}

long long compute_factorial(int n) {
    long long result = 1;
    for (int i = 1; i <= n; i++) {
        result *= i;
    }
    return result;
}

int main(void) {
    double a, b;
    int choice;
    enum Operation op;
    
    printf("Available operations:\n");
    printf("0: Add\n");
    printf("1: Subtract\n");
    printf("2: Multiply\n");
    printf("3: Divide\n");
    printf("4: Power\n");
    printf("5: Square Root\n");
    printf("6: Factorial\n");
    printf("Enter operation number: ");
    
    if (scanf("%d", &choice) != 1) {
        printf("Invalid input.\n");
        return 1;
    }
    
    if (choice < 0 || choice > 6) {
        printf("Invalid operation.\n");
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
                printf("Invalid input.\n");
                return 1;
            }
            printf("Enter second number: ");
            if (scanf("%lf", &b) != 1) {
                printf("Invalid input.\n");
                return 1;
            }
            break;
            
        case SQUARE_ROOT:
        case FACTORIAL:
            printf("Enter number: ");
            if (scanf("%lf", &a) != 1) {
                printf("Invalid input.\n");
                return 1;
            }
            break;
            
        default:
            printf("Invalid operation.\n");
            return 1;
    }
    
    switch (op) {
        case ADD:
            printf("Result: %.6f\n", a + b);
            break;
            
        case SUBTRACT:
            printf("Result: %.6f\n", a - b);
            break;
            
        case MULTIPLY:
            printf("Result: %.6f\n", a * b);
            break;
            
        case DIVIDE:
            if (fabs(b) < 1e-12) {
                printf("Error: Division by zero.\n");
                return 1;
            }
            printf("Result: %.6f\n", a / b);
            break;
            
        case POWER:
            if (a < 0 && fmod(b, 1.0) != 0.0) {
                printf("Error: Negative base with fractional exponent.\n");
                return 1;
            }
            if (a == 0 && b < 0) {
                printf("Error: Zero to negative power.\n");
                return 1;
            }
            printf("Result: %.6f\n", pow(a, b));
            break;
            
        case SQUARE_ROOT:
            if (a < 0) {
                printf("Error: Square root of negative number.\n");
                return 1;
            }
            printf("Result: %.6f\n", sqrt(a));
            break;
            
        case FACTORIAL:
            if (a != floor(a)) {
                printf("Error: Factorial requires integer.\n");
                return 1;
            }
            int n = (int)a;
            if (!validate_factorial_input(n)) {
                printf("Error: Factorial input out of range (0-12).\n");
                return 1;
            }
            printf("Result: %lld\n", compute_factorial(n));
            break;
            
        default:
            printf("Invalid operation.\n");
            return 1;
    }
    
    return 0;
}