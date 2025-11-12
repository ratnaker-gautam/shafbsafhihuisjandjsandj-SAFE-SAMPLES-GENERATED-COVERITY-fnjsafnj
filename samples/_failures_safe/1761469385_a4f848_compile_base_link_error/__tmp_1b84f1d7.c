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
        printf("Invalid operation selection\n");
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
            if (scanf("%lf", &a) != 1) {
                printf("Invalid number\n");
                return 1;
            }
            printf("Enter second number: ");
            if (scanf("%lf", &b) != 1) {
                printf("Invalid number\n");
                return 1;
            }
            break;
            
        case SQUARE_ROOT:
        case FACTORIAL:
            printf("Enter number: ");
            if (scanf("%lf", &a) != 1) {
                printf("Invalid number\n");
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
            if (fabs(b) < 1e-10) {
                printf("Error: Division by zero\n");
                return 1;
            }
            result = a / b;
            printf("%.2f / %.2f = %.2f\n", a, b, result);
            break;
            
        case POWER:
            if (a == 0 && b < 0) {
                printf("Error: Zero to negative power\n");
                return 1;
            }
            result = pow(a, b);
            printf("%.2f ^ %.2f = %.2f\n", a, b, result);
            break;
            
        case SQUARE_ROOT:
            if (a < 0) {
                printf("Error: Square root of negative number\n");
                return 1;
            }
            result = sqrt(a);
            printf("sqrt(%.2f) = %.2f\n", a, result);
            break;
            
        case FACTORIAL:
            if (a != (int)a || a < 0) {
                printf("Error: Factorial requires non-negative integer\n");
                return 1;
            }
            int_input = (int)a;
            if (!validate_factorial(int_input)) {
                printf("Error: Factorial input must be between 0 and 12\n");
                return 1;
            }
            long long fact_result = compute_factorial(int_input);
            printf("%d! = %lld\n", int_input, fact_result);
            break;
    }
    
    return 0;
}