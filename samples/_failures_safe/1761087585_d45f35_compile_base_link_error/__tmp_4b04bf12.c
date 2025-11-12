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

int parse_double(const char *str, double *result) {
    char *endptr;
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
    
    printf("Simple Numeric Calculator\n");
    printf("Enter expression (num1 operator num2): ");
    
    if (fgets(input, sizeof(input), stdin) == NULL) {
        fprintf(stderr, "Error reading input\n");
        return 1;
    }
    
    size_t len = strlen(input);
    if (len > 0 && input[len - 1] == '\n') {
        input[len - 1] = '\0';
    }
    
    if (strlen(input) == 0) {
        fprintf(stderr, "Empty input\n");
        return 1;
    }
    
    char num1_str[MAX_INPUT_LEN], num2_str[MAX_INPUT_LEN];
    int parsed = sscanf(input, "%255s %c %255s", num1_str, &op, num2_str);
    
    if (parsed != 3) {
        fprintf(stderr, "Invalid format. Use: number operator number\n");
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
        fprintf(stderr, "Invalid operator. Use: + - * / ^\n");
        return 1;
    }
    
    result = calculate_expression(num1, num2, op);
    
    if (isnan(result)) {
        if (op == '/' && fabs(num2) < 1e-12) {
            fprintf(stderr, "Division by zero\n");
        } else {
            fprintf(stderr, "Calculation error\n");
        }
        return 1;
    }
    
    if (result == HUGE_VAL || result == -HUGE_VAL || isnan(result)) {
        fprintf(stderr, "Result out of range\n");
        return 1;
    }
    
    printf("Result: %.6f\n", result);
    
    return 0;
}