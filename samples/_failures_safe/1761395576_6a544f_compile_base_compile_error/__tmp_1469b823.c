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

int parse_double(const char* str, double* result) {
    char* endptr;
    *result = strtod(str, &endptr);
    if (endptr == str || *endptr != '\0') {
        return 0;
    }
    return 1;
}

int main(void) {
    char input[MAX_INPUT_LEN];
    char op;
    double num1, num2, result;
    
    printf("Simple Calculator\n");
    printf("Supported operations: +, -, *, /, ^\n");
    printf("Enter expression (e.g., 5 + 3): ");
    
    if (fgets(input, sizeof(input), stdin) == NULL) {
        fprintf(stderr, "Error reading input\n");
        return 1;
    }
    
    size_t len = strlen(input);
    if (len > 0 && input[len - 1] == '\n') {
        input[len - 1] = '\0';
    }
    
    char num1_str[128], num2_str[128];
    int parsed = sscanf(input, "%127s %c %127s", num1_str, &op, num2_str);
    
    if (parsed != 3) {
        fprintf(stderr, "Invalid input format. Use: number operator number\n");
        return 1;
    }
    
    if (!parse_double(num1_str, &num1)) {
        fprintf(stderr, "Invalid first number\n");
        return 1;
    }
    
    if (!parse_double(num2_str, &num2)) {
        fprintf(stderr, "Invalid second number\n");
        return 1;
    }
    
    if (!is_valid_operator(op)) {
        fprintf(stderr, "Invalid operator\n");
        return 1;
    }
    
    result = calculate_expression(num1, num2, op);
    
    if (isnan(result)) {
        if (op == '/' && fabs(num2) < 1e-12) {
            fprintf(stderr, "Error: Division by zero\n");
        } else {
            fprintf(stderr, "Error: Invalid operation\n");
        }
        return 1;
    }
    
    if (result > DBL_MAX || result < -DBL_MAX) {
        fprintf(stderr, "Error: Result out of range\n");
        return 1;
    }
    
    printf("Result: %.6f\n", result);
    
    return 0;
}