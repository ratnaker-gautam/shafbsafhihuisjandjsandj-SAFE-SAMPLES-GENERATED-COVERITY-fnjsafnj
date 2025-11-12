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
    if (*result < -1e100 || *result > 1e100) {
        return 0;
    }
    return 1;
}

long computeFactorial(int n) {
    if (n < 0 || n > 20) {
        return -1;
    }
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
    enum Operation op;
    double num1, num2, result;
    long intResult;
    
    printf("Available operations:\n");
    printf("0: Add\n");
    printf("1: Subtract\n");
    printf("2: Multiply\n");
    printf("3: Divide\n");
    printf("4: Power\n");
    printf("5: Square Root\n");
    printf("6: Factorial\n");
    printf("Select operation (0-6): ");
    
    if (!fgets(input, sizeof(input), stdin)) {
        printf("Input error\n");
        return 1;
    }
    
    long choice;
    if (!validateIntegerInput(input, &choice)) {
        printf("Invalid operation selection\n");
        return 1;
    }
    
    op = (enum Operation)choice;
    if (op < ADD || op > FACTORIAL) {
        printf("Invalid operation\n");
        return 1;
    }
    
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
            break;
            
        case FACTORIAL:
            printf("Enter integer (0-20): ");
            if (!fgets(input, sizeof(input), stdin)) {
                printf("Input error\n");
                return 1;
            }
            break;
            
        default:
            printf("Invalid operation\n");
            return 1;
    }
    
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
            if (fabs(num2) < 1e-15) {
                printf("Error: Division by zero\n");
                return 1;
            }
            result = num1 / num2;
            printf("Result: %.6f\n", result);
            break;
            
        case POWER:
            if (num1 < 0 && fmod(num2, 1.0) != 0.0) {
                printf("Error: Negative base with fractional exponent\n");
                return 1;
            }
            result = pow(num1, num2);
            if (isnan(result) || isinf(result)) {
                printf("Error: Invalid power operation\n");
                return 1;
            }
            printf("Result: %.6f\n", result);
            break;
            
        case SQUARE_ROOT:
            if (num1 < 0) {
                printf("Error: Cannot compute square root of negative number\n");
                return 1;
            }
            result = sqrt(num1);
            printf("Result: %.6f\n", result);
            break;
            
        case FACTORIAL:
            if (!validateIntegerInput(input, &intResult)) {
                printf("Invalid integer input\n");
                return 1;
            }
            if (intResult < 0 || intResult > 20) {
                printf("Error: Factorial requires integer between 0 and 20\n");
                return 1;
            }
            long fact = computeFactorial((int)intResult);
            if (fact == -1) {
                printf("Error: Factorial computation