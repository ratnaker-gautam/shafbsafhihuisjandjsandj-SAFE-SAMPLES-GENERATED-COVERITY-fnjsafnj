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
            if (a < 0 && fmod(b, 1.0) != 0.0) {
                return NAN;
            }
            return pow(a, b);
        default: return NAN;
    }
}

int is_valid_operator(char c) {
    return c == '+' || c == '-' || c == '*' || c == '/' || c == '^';
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
    double num1, num2, result;
    char operator;
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
            len--;
        }
        
        if (len == 0) {
            continue;
        }
        
        if (strcmp(input, "quit") == 0) {
            break;
        }
        
        valid_input = parse_input(input, &num1, &num2, &operator);
        
        if (!valid_input) {
            printf("Error: Invalid input format. Use: number operator number\n");
            continue;
        }
        
        if (num1 < -1e100 || num1 > 1e100 || num2 < -1e100 || num2 > 1e100) {
            printf("Error: Numbers out of reasonable range\n");
            continue;
        }
        
        result = calculate_expression(num1, num2, operator);
        
        if (isnan(result)) {
            printf("Error: Mathematical error (division by zero or invalid operation)\n");
        } else if (isinf(result)) {
            printf("Error: Result is infinite\n");
        } else if (result < -1e100 || result > 1e100) {
            printf("Error: Result out of reasonable range\n");
        } else {
            printf("Result: %.15g\n", result);
        }
    }
    
    printf("Goodbye!\n");
    return 0;
}