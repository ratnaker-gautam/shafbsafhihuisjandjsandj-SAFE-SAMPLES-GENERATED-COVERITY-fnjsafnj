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

int validateIntegerInput(const char* input, long* result) {
    char* endptr;
    *result = strtol(input, &endptr, 10);
    if (endptr == input || *endptr != '\n') {
        return 0;
    }
    if (*result < -1000000 || *result > 1000000) {
        return 0;
    }
    return 1;
}

int validateDoubleInput(const char* input, double* result) {
    char* endptr;
    *result = strtod(input, &endptr);
    if (endptr == input || *endptr != '\n') {
        return 0;
    }
    if (*result < -1e6 || *result > 1e6) {
        return 0;
    }
    return 1;
}

long computeFactorial(int n) {
    if (n < 0) return -1;
    if (n == 0 || n == 1) return 1;
    long result = 1;
    for (int i = 2; i <= n; i++) {
        if (result > LONG_MAX / i) {
            return -1;
        }
        result *= i;
    }
    return result;
}

int main() {
    char input[256];
    enum Operation op = INVALID;
    double num1 = 0.0, num2 = 0.0;
    long intNum = 0;
    
    printf("Available operations:\n");
    printf("0: Addition\n");
    printf("1: Subtraction\n");
    printf("2: Multiplication\n");
    printf("3: Division\n");
    printf("4: Power\n");
    printf("5: Square Root\n");
    printf("6: Factorial\n");
    printf("Select operation (0-6): ");
    
    if (!fgets(input, sizeof(input), stdin)) {
        printf("Input error\n");
        return 1;
    }
    
    char* endptr;
    long choice = strtol(input, &endptr, 10);
    if (endptr == input || *endptr != '\n') {
        printf("Invalid input format\n");
        return 1;
    }
    
    if (choice < 0 || choice > 6) {
        printf("Invalid operation selection\n");
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
            if (!fgets(input, sizeof(input), stdin) || !validateDoubleInput(input, &num1)) {
                printf("Invalid first number\n");
                return 1;
            }
            
            printf("Enter second number: ");
            if (!fgets(input, sizeof(input), stdin) || !validateDoubleInput(input, &num2)) {
                printf("Invalid second number\n");
                return 1;
            }
            break;
            
        case SQUARE_ROOT:
            printf("Enter number: ");
            if (!fgets(input, sizeof(input), stdin) || !validateDoubleInput(input, &num1)) {
                printf("Invalid number\n");
                return 1;
            }
            if (num1 < 0) {
                printf("Cannot calculate square root of negative number\n");
                return 1;
            }
            break;
            
        case FACTORIAL:
            printf("Enter integer (0-20): ");
            if (!fgets(input, sizeof(input), stdin) || !validateIntegerInput(input, &intNum)) {
                printf("Invalid integer\n");
                return 1;
            }
            if (intNum < 0 || intNum > 20) {
                printf("Factorial only defined for 0-20\n");
                return 1;
            }
            break;
            
        default:
            printf("Invalid operation\n");
            return 1;
    }
    
    double result = 0.0;
    long factResult = 0;
    
    switch (op) {
        case ADD:
            result = num1 + num2;
            printf("Result: %.6f\n", result);
            break;
            
        case SUBTRACT:
            result = num1 - num2;
            printf("Result: %.6f\n", result);
            break;
            
        case MULTIPLY:
            result = num1 * num2;
            printf("Result: %.6f\n", result);
            break;
            
        case DIVIDE:
            if (fabs(num2) < 1e-12) {
                printf("Division by zero\n");
                return 1;
            }
            result = num1 / num2;
            printf("Result: %.6f\n", result);
            break;
            
        case POWER:
            result = pow(num1, num2);
            if (isnan(result) || isinf(result)) {
                printf("Power calculation error\n");
                return 1;
            }
            printf("Result: %.6f\n", result);
            break;
            
        case SQUARE_ROOT:
            result = sqrt(num1);
            printf("Result: %.6f\n", result);
            break;
            
        case FACTORIAL:
            factResult = computeFactorial((int)intNum);
            if (factResult == -1) {
                printf("Factorial calculation error\n");
                return 1;