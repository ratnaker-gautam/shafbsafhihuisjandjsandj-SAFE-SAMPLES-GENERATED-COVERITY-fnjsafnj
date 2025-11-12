//DeepSeek-V3 V2.5 Category: Safe ; Style: io_bound ; Variation: numeric_computation
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <stdint.h>
#include <limits.h>

#define MAX_INPUT_LEN 256

double calculate_expression(double a, double b, char op) {
    switch (op) {
        case '+': return a + b;
        case '-': return a - b;
        case '*': return a * b;
        case '/': 
            if (fabs(b) < 1e-12) {
                return NAN;
            }
            return a / b;
        case '^': 
            if (a < 0 && fmod(b, 1.0) != 0) {
                return NAN;
            }
            return pow(a, b);
        default: return NAN;
    }
}

int is_valid_operator(char op) {
    return op == '+' || op == '-' || op == '*' || op == '/' || op == '^';
}

int parse_input(const char* input, double* a, double* b, char* op) {
    char op_str[2] = {0};
    int result = sscanf(input, "%lf %1[+\\-*/^] %lf", a, op_str, b);
    if (result != 3) {
        return 0;
    }
    *op = op_str[0];
    return is_valid_operator(*op);
}

int main(void) {
    char input[MAX_INPUT_LEN + 1];
    double num1, num2;
    char operation;
    
    printf("Numeric Expression Calculator\n");
    printf("Enter expressions in format: number operator number\n");
    printf("Supported operators: + - * / ^\n");
    printf("Enter 'quit' to exit\n\n");
    
    while (1) {
        printf("> ");
        
        if (fgets(input, sizeof(input), stdin) == NULL) {
            break;
        }
        
        size_t len = strlen(input);
        if (len > 0 && input[len - 1] == '\n') {
            input[len - 1] = '\0';
            len--;
        }
        
        if (len == 0) {
            continue;
        }
        
        if (strcmp(input, "quit") == 0) {
            break;
        }
        
        if (len >= MAX_INPUT_LEN) {
            printf("Error: Input too long\n");
            continue;
        }
        
        if (!parse_input(input, &num1, &num2, &operation)) {
            printf("Error: Invalid format. Use: number operator number\n");
            continue;
        }
        
        if (!isfinite(num1) || !isfinite(num2)) {
            printf("Error: Invalid numbers\n");
            continue;
        }
        
        double result = calculate_expression(num1, num2, operation);
        
        if (isnan(result)) {
            printf("Error: Invalid operation");
            if (operation == '/' && fabs(num2) < 1e-12) {
                printf(" (division by zero)");
            } else if (operation == '^' && num1 < 0 && fmod(num2, 1.0) != 0) {
                printf(" (negative base with fractional exponent)");
            }
            printf("\n");
        } else if (!isfinite(result)) {
            printf("Error: Result is not finite\n");
        } else {
            printf("Result: %.15g\n", result);
        }
    }
    
    printf("Goodbye!\n");
    return 0;
}