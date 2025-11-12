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
            if (b == 0.0) {
                return NAN;
            }
            return a / b;
        case '^': return pow(a, b);
        default: return NAN;
    }
}

int is_valid_operator(char c) {
    return c == '+' || c == '-' || c == '*' || c == '/' || c == '^';
}

int parse_input(const char* input, double* a, double* b, char* op) {
    char op_str[2];
    int result = sscanf(input, "%lf %1[+\\-*/^] %lf", a, op_str, b);
    if (result != 3) {
        return 0;
    }
    *op = op_str[0];
    return is_valid_operator(*op);
}

int main(void) {
    char input[MAX_INPUT_LEN + 1];
    double a, b;
    char op;
    double result;
    
    printf("Enter expression (format: number operator number): ");
    
    if (fgets(input, sizeof(input), stdin) == NULL) {
        fprintf(stderr, "Error reading input\n");
        return 1;
    }
    
    size_t len = strlen(input);
    if (len > 0 && input[len - 1] == '\n') {
        input[len - 1] = '\0';
    }
    
    if (len == 0 || len >= MAX_INPUT_LEN) {
        fprintf(stderr, "Invalid input length\n");
        return 1;
    }
    
    if (!parse_input(input, &a, &b, &op)) {
        fprintf(stderr, "Invalid expression format\n");
        return 1;
    }
    
    if (isnan(a) || isnan(b) || isinf(a) || isinf(b)) {
        fprintf(stderr, "Invalid numeric values\n");
        return 1;
    }
    
    result = calculate_expression(a, b, op);
    
    if (isnan(result)) {
        if (op == '/' && b == 0.0) {
            fprintf(stderr, "Division by zero\n");
        } else {
            fprintf(stderr, "Calculation error\n");
        }
        return 1;
    }
    
    if (isinf(result)) {
        fprintf(stderr, "Result out of representable range\n");
        return 1;
    }
    
    printf("Result: %.6f\n", result);
    
    return 0;
}