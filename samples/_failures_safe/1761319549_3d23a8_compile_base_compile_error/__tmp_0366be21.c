//DeepSeek-V3 V2.5 Category: Safe ; Style: enum_switch ; Variation: numeric_computation
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <limits.h>

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

int validateInteger(double value) {
    return (value >= 0.0 && value == floor(value) && value <= 20.0);
}

long long computeFactorial(int n) {
    long long result = 1;
    for (int i = 1; i <= n; i++) {
        if (result > LLONG_MAX / i) {
            return -1;
        }
        result *= i;
    }
    return result;
}

int main() {
    double a, b;
    int op_choice;
    enum Operation operation;
    
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
    
    printf("Select operation:\n");
    printf("0: Add\n");
    printf("1: Subtract\n");
    printf("2: Multiply\n");
    printf("3: Divide\n");
    printf("4: Power\n");
    printf("5: Square Root (uses first number)\n");
    printf("6: Factorial (uses first number)\n");
    printf("Enter choice (0-6): ");
    
    if (scanf("%d", &op_choice) != 1) {
        printf("Invalid operation choice\n");
        return 1;
    }
    
    if (op_choice < 0 || op_choice > 6) {
        printf("Operation choice out of range\n");
        return 1;
    }
    
    operation = (enum Operation)op_choice;
    
    switch (operation) {
        case ADD:
            if (fabs(a) > 1e100 || fabs(b) > 1e100) {
                printf("Input values too large for addition\n");
                return 1;
            }
            printf("Result: %.6lf\n", a + b);
            break;
            
        case SUBTRACT:
            if (fabs(a) > 1e100 || fabs(b) > 1e100) {
                printf("Input values too large for subtraction\n");
                return 1;
            }
            printf("Result: %.6lf\n", a - b);
            break;
            
        case MULTIPLY:
            if (fabs(a) > 1e50 || fabs(b) > 1e50) {
                printf("Input values too large for multiplication\n");
                return 1;
            }
            printf("Result: %.6lf\n", a * b);
            break;
            
        case DIVIDE:
            if (fabs(b) < 1e-15) {
                printf("Division by zero error\n");
                return 1;
            }
            if (fabs(a) > 1e100 || fabs(b) > 1e100) {
                printf("Input values too large for division\n");
                return 1;
            }
            printf("Result: %.6lf\n", a / b);
            break;
            
        case POWER:
            if (!validateInteger(b)) {
                printf("Exponent must be a non-negative integer <= 20\n");
                return 1;
            }
            if (fabs(a) > 1000.0 && (int)b > 0) {
                printf("Base too large for power operation\n");
                return 1;
            }
            printf("Result: %.6lf\n", pow(a, b));
            break;
            
        case SQUARE_ROOT:
            if (a < 0.0) {
                printf("Cannot compute square root of negative number\n");
                return 1;
            }
            if (a > 1e100) {
                printf("Input value too large for square root\n");
                return 1;
            }
            printf("Result: %.6lf\n", sqrt(a));
            break;
            
        case FACTORIAL:
            if (!validateInteger(a)) {
                printf("Factorial requires non-negative integer <= 20\n");
                return 1;
            }
            long long fact = computeFactorial((int)a);
            if (fact == -1) {
                printf("Factorial computation overflow\n");
                return 1;
            }
            printf("Result: %lld\n", fact);
            break;
            
        default:
            printf("Invalid operation\n");
            return 1;
    }
    
    return 0;
}