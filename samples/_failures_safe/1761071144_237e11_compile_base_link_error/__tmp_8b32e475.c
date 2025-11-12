//DeepSeek-V3 SAFE v1.4 Category: Safe ; Style: io_bound ; Variation: numeric_computation
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
    int valid_input;
    
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
        
        if (strlen(input) == 0) {
            continue;
        }
        
        valid_input = parse_input(input, &a, &b, &op);
        
        if (!valid_input) {
            printf("Error: Invalid input format. Use: number operator number\n");
            continue;
        }
        
        if (!isfinite(a) || !isfinite(b)) {
            printf("Error: Numbers must be finite\n");
            continue;
        }
        
        double result = calculate_expression(a, b, op);
        
        if (isnan(result)) {
            if (op == '/' && fabs(b) < 1e-12) {
                printf("Error: Division by zero\n");
            } else {
                printf("Error: Invalid operation\n");
            }
        } else if (!isfinite(result)) {
            printf("Error: Result is not finite\n");
        } else {
            printf("Result: %.6f\n", result);
        }
    }
    
    printf("Goodbye!\n");
    return 0;
}