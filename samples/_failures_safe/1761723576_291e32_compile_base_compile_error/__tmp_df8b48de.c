//DeepSeek-V3 V2.5 Category: Safe ; Style: io_bound ; Variation: numeric_computation
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <stdint.h>
#include <limits.h>
#include <float.h>

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

int parse_double(const char *str, double *result) {
    char *endptr;
    *result = strtod(str, &endptr);
    return endptr != str && *endptr == '\0';
}

int main(void) {
    char input[MAX_INPUT_LEN];
    char op;
    double num1, num2, result;
    char str1[MAX_INPUT_LEN], str2[MAX_INPUT_LEN];
    
    printf("Simple Numeric Calculator\n");
    printf("Enter expressions in format: number operator number\n");
    printf("Supported operators: + - * / ^\n");
    printf("Enter 'quit' to exit\n\n");
    
    while (1) {
        printf("> ");
        
        if (fgets(input, sizeof(input), stdin) == NULL) {
            break;
        }
        
        input[strcspn(input, "\n")] = '\0';
        
        if (strcmp(input, "quit") == 0) {
            break;
        }
        
        if (strlen(input) == 0) {
            continue;
        }
        
        int parsed = sscanf(input, "%255s %c %255s", str1, &op, str2);
        
        if (parsed != 3) {
            printf("Error: Invalid format. Use: number operator number\n");
            continue;
        }
        
        if (!parse_double(str1, &num1)) {
            printf("Error: Invalid first number: %s\n", str1);
            continue;
        }
        
        if (!parse_double(str2, &num2)) {
            printf("Error: Invalid second number: %s\n", str2);
            continue;
        }
        
        if (!is_valid_operator(op)) {
            printf("Error: Invalid operator: %c\n", op);
            continue;
        }
        
        result = calculate_expression(num1, num2, op);
        
        if (isnan(result)) {
            if (op == '/' && fabs(num2) < 1e-12) {
                printf("Error: Division by zero\n");
            } else {
                printf("Error: Invalid operation\n");
            }
            continue;
        }
        
        if (result > DBL_MAX || result < -DBL_MAX) {
            printf("Error: Result out of range\n");
            continue;
        }
        
        printf("Result: %.6g\n", result);
    }
    
    printf("Goodbye!\n");
    return 0;
}