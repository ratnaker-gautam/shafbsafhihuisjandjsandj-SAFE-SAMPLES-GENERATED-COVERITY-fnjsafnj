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
    for (int i = 1; i <= n; i++) {
        result *= i;
    }
    return result;
}

int main(void) {
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
    double result = 0.0;
    
    switch (op) {
        case ADD:
            result = a + b;
            printf("Result: %.2lf + %.2lf = %.2lf\n", a, b, result);
            break;
            
        case SUBTRACT:
            result = a - b;
            printf("Result: %.2lf - %.2lf = %.2lf\n", a, b, result);
            break;
            
        case MULTIPLY:
            result = a * b;
            printf("Result: %.2lf * %.2lf = %.2lf\n", a, b, result);
            break;
            
        case DIVIDE:
            if (fabs(b) < 1e-10) {
                printf("Error: Division by zero\n");
                return 1;
            }
            result = a / b;
            printf("Result: %.2lf / %.2lf = %.2lf\n", a, b, result);
            break;
            
        case POWER:
            if (a < 0 && fmod(b, 1.0) != 0.0) {
                printf("Error: Negative base with fractional exponent\n");
                return 1;
            }
            result = pow(a, b);
            printf("Result: %.2lf ^ %.2lf = %.2lf\n", a, b, result);
            break;
            
        case SQUARE_ROOT:
            if (a < 0) {
                printf("Error: Cannot compute square root of negative number\n");
                return 1;
            }
            result = sqrt(a);
            printf("Result: sqrt(%.2lf) = %.2lf\n", a, result);
            break;
            
        case FACTORIAL:
            if (fmod(a, 1.0) != 0.0 || a < 0) {
                printf("Error: Factorial requires non-negative integer\n");
                return 1;
            }
            int n = (int)a;
            if (!validate_factorial(n)) {
                printf("Error: Factorial input must be between 0 and 12\n");
                return 1;
            }
            long long fact_result = compute_factorial(n);
            printf("Result: %d! = %lld\n", n, fact_result);
            break;
            
        default:
            printf("Invalid operation\n");
            return 1;
    }
    
    return 0;
}