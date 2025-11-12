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
    for (int i = 1; i <= n; i++) {
        if (result > 1e308 / i) return -1.0;
        result *= i;
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
            printf("Result: %.6lf\n", result);
            break;
            
        case SUBTRACT:
            result = a - b;
            printf("Result: %.6lf\n", result);
            break;
            
        case MULTIPLY:
            result = a * b;
            printf("Result: %.6lf\n", result);
            break;
            
        case DIVIDE:
            if (fabs(b) < 1e-15) {
                printf("Error: Division by zero\n");
                return 1;
            }
            result = a / b;
            printf("Result: %.6lf\n", result);
            break;
            
        case POWER:
            if (a < 0 && fmod(b, 1.0) != 0.0) {
                printf("Error: Negative base with fractional exponent\n");
                return 1;
            }
            result = pow(a, b);
            if (isnan(result) || isinf(result)) {
                printf("Error: Invalid power operation\n");
                return 1;
            }
            printf("Result: %.6lf\n", result);
            break;
            
        case SQUARE_ROOT:
            if (a < 0) {
                printf("Error: Cannot compute square root of negative number\n");
                return 1;
            }
            result = sqrt(a);
            printf("Result: %.6lf\n", result);
            break;
            
        case FACTORIAL:
            if (a < 0 || a != floor(a)) {
                printf("Error: Factorial requires non-negative integer\n");
                return 1;
            }
            if (a > 170) {
                printf("Error: Number too large for factorial\n");
                return 1;
            }
            result = compute_factorial((int)a);
            if (result < 0) {
                printf("Error: Factorial computation failed\n");
                return 1;
            }
            printf("Result: %.0lf\n", result);
            break;
    }
    
    return 0;
}