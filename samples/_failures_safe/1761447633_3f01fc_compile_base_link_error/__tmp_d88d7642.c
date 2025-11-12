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
    printf("0: Add\n");
    printf("1: Subtract\n");
    printf("2: Multiply\n");
    printf("3: Divide\n");
    printf("4: Power\n");
    printf("5: Square Root\n");
    printf("6: Factorial\n");
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
        case FACTORIAL:
            printf("Enter number: ");
            if (scanf("%lf", &a) != 1) {
                printf("Invalid input\n");
                return 1;
            }
            break;
    }
    
    switch (op) {
        case ADD:
            result = a + b;
            printf("%.2f + %.2f = %.2f\n", a, b, result);
            break;
            
        case SUBTRACT:
            result = a - b;
            printf("%.2f - %.2f = %.2f\n", a, b, result);
            break;
            
        case MULTIPLY:
            result = a * b;
            printf("%.2f * %.2f = %.2f\n", a, b, result);
            break;
            
        case DIVIDE:
            if (b == 0.0) {
                printf("Error: Division by zero\n");
                return 1;
            }
            result = a / b;
            printf("%.2f / %.2f = %.2f\n", a, b, result);
            break;
            
        case POWER:
            if (a == 0.0 && b < 0.0) {
                printf("Error: 0 to negative power undefined\n");
                return 1;
            }
            result = pow(a, b);
            if (isinf(result)) {
                printf("Error: Result overflow\n");
                return 1;
            }
            if (isnan(result)) {
                printf("Error: Invalid power operation\n");
                return 1;
            }
            printf("%.2f ^ %.2f = %.2f\n", a, b, result);
            break;
            
        case SQUARE_ROOT:
            if (a < 0.0) {
                printf("Error: Square root of negative number\n");
                return 1;
            }
            result = sqrt(a);
            printf("sqrt(%.2f) = %.2f\n", a, result);
            break;
            
        case FACTORIAL:
            if (a != (int)a || a < 0.0) {
                printf("Error: Factorial requires non-negative integer\n");
                return 1;
            }
            result = compute_factorial((int)a);
            if (isinf(result)) {
                printf("Error: Factorial overflow\n");
                return 1;
            }
            if (isnan(result)) {
                printf("Error: Invalid factorial input\n");
                return 1;
            }
            printf("%d! = %.0f\n", (int)a, result);
            break;
    }
    
    return 0;
}