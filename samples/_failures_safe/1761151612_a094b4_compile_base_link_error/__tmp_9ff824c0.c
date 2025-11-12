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

int main() {
    enum Operation op;
    double a, b, result;
    int int_input;
    char op_input;
    
    printf("Available operations:\n");
    printf("+ : Addition\n");
    printf("- : Subtraction\n");
    printf("* : Multiplication\n");
    printf("/ : Division\n");
    printf("^ : Power\n");
    printf("s : Square Root\n");
    printf("! : Factorial\n");
    printf("Enter operation: ");
    
    if (scanf(" %c", &op_input) != 1) {
        printf("Invalid input\n");
        return 1;
    }
    
    switch (op_input) {
        case '+': op = ADD; break;
        case '-': op = SUBTRACT; break;
        case '*': op = MULTIPLY; break;
        case '/': op = DIVIDE; break;
        case '^': op = POWER; break;
        case 's': op = SQUARE_ROOT; break;
        case '!': op = FACTORIAL; break;
        default:
            printf("Invalid operation\n");
            return 1;
    }
    
    if (op == SQUARE_ROOT || op == FACTORIAL) {
        if (op == SQUARE_ROOT) {
            printf("Enter number: ");
            if (scanf("%lf", &a) != 1) {
                printf("Invalid input\n");
                return 1;
            }
            if (a < 0) {
                printf("Cannot compute square root of negative number\n");
                return 1;
            }
            result = sqrt(a);
        } else {
            printf("Enter integer (0-12): ");
            if (scanf("%d", &int_input) != 1) {
                printf("Invalid input\n");
                return 1;
            }
            if (!validate_factorial(int_input)) {
                printf("Factorial input must be between 0 and 12\n");
                return 1;
            }
            result = (double)compute_factorial(int_input);
        }
    } else {
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
        
        switch (op) {
            case ADD:
                result = a + b;
                break;
            case SUBTRACT:
                result = a - b;
                break;
            case MULTIPLY:
                result = a * b;
                break;
            case DIVIDE:
                if (b == 0) {
                    printf("Division by zero\n");
                    return 1;
                }
                result = a / b;
                break;
            case POWER:
                if (a == 0 && b < 0) {
                    printf("Undefined: 0 to negative power\n");
                    return 1;
                }
                result = pow(a, b);
                break;
            default:
                printf("Invalid operation\n");
                return 1;
        }
    }
    
    printf("Result: %.6f\n", result);
    return 0;
}