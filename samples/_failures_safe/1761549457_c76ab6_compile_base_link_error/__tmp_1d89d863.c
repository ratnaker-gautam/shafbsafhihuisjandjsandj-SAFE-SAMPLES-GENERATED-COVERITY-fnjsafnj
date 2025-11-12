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
    MODULO
};

int main() {
    int choice;
    double a, b, result;
    int ia, ib, iresult;
    
    printf("Available operations:\n");
    printf("0: Add\n");
    printf("1: Subtract\n");
    printf("2: Multiply\n");
    printf("3: Divide\n");
    printf("4: Power\n");
    printf("5: Square Root\n");
    printf("6: Factorial\n");
    printf("7: Modulo\n");
    printf("Select operation (0-7): ");
    
    if (scanf("%d", &choice) != 1) {
        printf("Invalid input\n");
        return 1;
    }
    
    if (choice < 0 || choice > 7) {
        printf("Invalid operation\n");
        return 1;
    }
    
    switch (choice) {
        case ADD:
            printf("Enter two numbers: ");
            if (scanf("%lf %lf", &a, &b) != 2) {
                printf("Invalid input\n");
                return 1;
            }
            result = a + b;
            printf("Result: %.2f\n", result);
            break;
            
        case SUBTRACT:
            printf("Enter two numbers: ");
            if (scanf("%lf %lf", &a, &b) != 2) {
                printf("Invalid input\n");
                return 1;
            }
            result = a - b;
            printf("Result: %.2f\n", result);
            break;
            
        case MULTIPLY:
            printf("Enter two numbers: ");
            if (scanf("%lf %lf", &a, &b) != 2) {
                printf("Invalid input\n");
                return 1;
            }
            result = a * b;
            printf("Result: %.2f\n", result);
            break;
            
        case DIVIDE:
            printf("Enter two numbers: ");
            if (scanf("%lf %lf", &a, &b) != 2) {
                printf("Invalid input\n");
                return 1;
            }
            if (fabs(b) < 1e-10) {
                printf("Error: Division by zero\n");
                return 1;
            }
            result = a / b;
            printf("Result: %.2f\n", result);
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
            printf("Result: %.2f\n", result);
            break;
            
        case SQUARE_ROOT:
            printf("Enter a number: ");
            if (scanf("%lf", &a) != 1) {
                printf("Invalid input\n");
                return 1;
            }
            if (a < 0) {
                printf("Error: Cannot calculate square root of negative number\n");
                return 1;
            }
            result = sqrt(a);
            printf("Result: %.2f\n", result);
            break;
            
        case FACTORIAL:
            printf("Enter a non-negative integer: ");
            if (scanf("%d", &ia) != 1) {
                printf("Invalid input\n");
                return 1;
            }
            if (ia < 0) {
                printf("Error: Factorial of negative number is undefined\n");
                return 1;
            }
            if (ia > 12) {
                printf("Error: Result would overflow 32-bit integer\n");
                return 1;
            }
            iresult = 1;
            for (int i = 2; i <= ia; i++) {
                iresult *= i;
            }
            printf("Result: %d\n", iresult);
            break;
            
        case MODULO:
            printf("Enter two integers: ");
            if (scanf("%d %d", &ia, &ib) != 2) {
                printf("Invalid input\n");
                return 1;
            }
            if (ib == 0) {
                printf("Error: Division by zero\n");
                return 1;
            }
            iresult = ia % ib;
            printf("Result: %d\n", iresult);
            break;
    }
    
    return 0;
}