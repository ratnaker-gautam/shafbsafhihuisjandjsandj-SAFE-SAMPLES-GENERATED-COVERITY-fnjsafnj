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

int validate_factorial(int n) {
    if (n < 0 || n > 12) {
        return 0;
    }
    return 1;
}

long long compute_factorial(int n) {
    long long result = 1;
    for (int i = 2; i <= n; i++) {
        result *= i;
    }
    return result;
}

int main() {
    double a, b;
    int op_choice;
    enum Operation op;
    
    printf("Enter first number: ");
    if (scanf("%lf", &a) != 1) {
        printf("Invalid input for first number\n");
        return 1;
    }
    
    printf("Enter second number: ");
    if (scanf("%lf", &b) != 1) {
        printf("Invalid input for second number\n");
        return 1;
    }
    
    printf("Available operations:\n");
    printf("0: Add\n");
    printf("1: Subtract\n");
    printf("2: Multiply\n");
    printf("3: Divide\n");
    printf("4: Power\n");
    printf("5: Square Root (uses first number)\n");
    printf("6: Factorial (uses first number as integer)\n");
    printf("Choose operation (0-6): ");
    
    if (scanf("%d", &op_choice) != 1) {
        printf("Invalid operation choice\n");
        return 1;
    }
    
    if (op_choice < 0 || op_choice > 6) {
        printf("Operation choice out of range\n");
        return 1;
    }
    
    op = (enum Operation)op_choice;
    
    switch (op) {
        case ADD:
            printf("Result: %.6lf\n", a + b);
            break;
            
        case SUBTRACT:
            printf("Result: %.6lf\n", a - b);
            break;
            
        case MULTIPLY:
            printf("Result: %.6lf\n", a * b);
            break;
            
        case DIVIDE:
            if (fabs(b) < 1e-12) {
                printf("Error: Division by zero\n");
                return 1;
            }
            printf("Result: %.6lf\n", a / b);
            break;
            
        case POWER:
            if (a < 0 && fmod(b, 1.0) != 0.0) {
                printf("Error: Negative base with fractional exponent\n");
                return 1;
            }
            printf("Result: %.6lf\n", pow(a, b));
            break;
            
        case SQUARE_ROOT:
            if (a < 0) {
                printf("Error: Cannot compute square root of negative number\n");
                return 1;
            }
            printf("Result: %.6lf\n", sqrt(a));
            break;
            
        case FACTORIAL:
            {
                int n = (int)a;
                if (a != (double)n) {
                    printf("Error: Factorial requires integer input\n");
                    return 1;
                }
                if (!validate_factorial(n)) {
                    printf("Error: Factorial input must be between 0 and 12\n");
                    return 1;
                }
                printf("Result: %lld\n", compute_factorial(n));
            }
            break;
            
        default:
            printf("Invalid operation\n");
            return 1;
    }
    
    return 0;
}