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
    if (n < 0) return 0;
    if (n > 12) return 0;
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
    enum Operation op;
    double a, b, result;
    int int_input;
    char op_char;
    
    printf("Available operations:\n");
    printf("+ : Addition\n");
    printf("- : Subtraction\n");
    printf("* : Multiplication\n");
    printf("/ : Division\n");
    printf("^ : Power\n");
    printf("s : Square Root\n");
    printf("! : Factorial\n");
    printf("Select operation: ");
    
    if (scanf(" %c", &op_char) != 1) {
        printf("Error: Invalid input\n");
        return 1;
    }
    
    switch (op_char) {
        case '+': op = ADD; break;
        case '-': op = SUBTRACT; break;
        case '*': op = MULTIPLY; break;
        case '/': op = DIVIDE; break;
        case '^': op = POWER; break;
        case 's': op = SQUARE_ROOT; break;
        case '!': op = FACTORIAL; break;
        default:
            printf("Error: Invalid operation\n");
            return 1;
    }
    
    if (op == SQUARE_ROOT || op == FACTORIAL) {
        printf("Enter number: ");
        if (scanf("%d", &int_input) != 1) {
            printf("Error: Invalid input\n");
            return 1;
        }
        
        if (op == SQUARE_ROOT) {
            if (int_input < 0) {
                printf("Error: Cannot compute square root of negative number\n");
                return 1;
            }
            result = sqrt(int_input);
            printf("Square root of %d = %.6f\n", int_input, result);
        } else {
            if (!validate_factorial(int_input)) {
                printf("Error: Factorial input must be between 0 and 12\n");
                return 1;
            }
            long long fact_result = compute_factorial(int_input);
            printf("%d! = %lld\n", int_input, fact_result);
        }
    } else {
        printf("Enter first number: ");
        if (scanf("%lf", &a) != 1) {
            printf("Error: Invalid input\n");
            return 1;
        }
        
        printf("Enter second number: ");
        if (scanf("%lf", &b) != 1) {
            printf("Error: Invalid input\n");
            return 1;
        }
        
        switch (op) {
            case ADD:
                result = a + b;
                printf("%.6f + %.6f = %.6f\n", a, b, result);
                break;
            case SUBTRACT:
                result = a - b;
                printf("%.6f - %.6f = %.6f\n", a, b, result);
                break;
            case MULTIPLY:
                result = a * b;
                printf("%.6f * %.6f = %.6f\n", a, b, result);
                break;
            case DIVIDE:
                if (fabs(b) < 1e-12) {
                    printf("Error: Division by zero\n");
                    return 1;
                }
                result = a / b;
                printf("%.6f / %.6f = %.6f\n", a, b, result);
                break;
            case POWER:
                if (a < 0 && fmod(b, 1.0) != 0.0) {
                    printf("Error: Negative base with fractional exponent\n");
                    return 1;
                }
                if (a == 0 && b < 0) {
                    printf("Error: Zero to negative power\n");
                    return 1;
                }
                result = pow(a, b);
                printf("%.6f ^ %.6f = %.6f\n", a, b, result);
                break;
            default:
                printf("Error: Invalid operation\n");
                return 1;
        }
    }
    
    return 0;
}