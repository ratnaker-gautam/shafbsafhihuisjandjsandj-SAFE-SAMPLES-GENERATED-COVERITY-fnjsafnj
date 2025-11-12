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

int is_valid_operator(char c) {
    return c == '+' || c == '-' || c == '*' || c == '/' || c == '^';
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
    char input[MAX_INPUT_LEN + 1];
    double num1, num2;
    char operation;
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
    
    if (len == 1 && input[0] == '\0') {
        fprintf(stderr, "Empty input\n");
        return 1;
    }
    
    if (!parse_input(input, &num1, &num2, &operation)) {
        fprintf(stderr, "Invalid input format. Use: number operator number\n");
        fprintf(stderr, "Valid operators: + - * / ^\n");
        return 1;
    }
    
    if (num1 < -1e100 || num1 > 1e100 || num2 < -1e100 || num2 > 1e100) {
        fprintf(stderr, "Numbers out of acceptable range\n");
        return 1;
    }
    
    result = calculate_expression(num1, num2, operation);
    
    if (isnan(result)) {
        if (operation == '/' && fabs(num2) < 1e-12) {
            fprintf(stderr, "Error: Division by zero\n");
        } else if (operation == '^' && num1 < 0 && fmod(num2, 1.0) != 0) {
            fprintf(stderr, "Error: Negative base with fractional exponent\n");
        } else {
            fprintf(stderr, "Error: Invalid operation\n");
        }
        return 1;
    }
    
    if (result > 1e100 || result < -1e100 || (fabs(result) < 1e-100 && result != 0)) {
        fprintf(stderr, "Result out of representable range\n");
        return 1;
    }
    
    printf("Result: %.6g\n", result);
    
    return 0;
}