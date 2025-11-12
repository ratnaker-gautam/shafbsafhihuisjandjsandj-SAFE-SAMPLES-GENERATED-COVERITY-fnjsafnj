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
        case '^': return pow(a, b);
        default: return NAN;
    }
}

int is_valid_operator(char op) {
    return op == '+' || op == '-' || op == '*' || op == '/' || op == '^';
}

int parse_input(const char* input, double* a, double* b, char* op) {
    char op_str[2];
    int parsed = sscanf(input, "%lf %1[+\\-*/^] %lf", a, op_str, b);
    if (parsed != 3) {
        return 0;
    }
    *op = op_str[0];
    return is_valid_operator(*op);
}

int main(void) {
    char input[MAX_INPUT_LEN];
    double a, b;
    char op;
    
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
        }
        
        if (strcmp(input, "quit") == 0) {
            break;
        }
        
        if (parse_input(input, &a, &b, &op)) {
            double result = calculate_expression(a, b, op);
            
            if (isnan(result)) {
                if (op == '/' && fabs(b) < 1e-12) {
                    printf("Error: Division by zero\n");
                } else {
                    printf("Error: Invalid operation\n");
                }
            } else if (isinf(result)) {
                printf("Error: Result is infinite\n");
            } else {
                printf("Result: %.6f\n", result);
            }
        } else {
            printf("Error: Invalid input format. Use: number operator number\n");
        }
    }
    
    printf("Goodbye!\n");
    return 0;
}