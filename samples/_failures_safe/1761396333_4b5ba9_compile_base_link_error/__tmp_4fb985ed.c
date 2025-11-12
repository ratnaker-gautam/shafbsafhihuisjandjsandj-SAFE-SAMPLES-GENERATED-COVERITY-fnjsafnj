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
    char num1_str[MAX_INPUT_LEN], num2_str[MAX_INPUT_LEN];
    
    printf("Simple Numeric Calculator\n");
    printf("Enter expression (num operator num): ");
    
    if (fgets(input, sizeof(input), stdin) == NULL) {
        fprintf(stderr, "Error reading input\n");
        return 1;
    }
    
    input[strcspn(input, "\n")] = '\0';
    
    int parsed = sscanf(input, "%255s %c %255s", num1_str, &op, num2_str);
    if (parsed != 3) {
        fprintf(stderr, "Invalid input format. Use: number operator number\n");
        return 1;
    }
    
    if (!parse_double(num1_str, &num1)) {
        fprintf(stderr, "Invalid first number: %s\n", num1_str);
        return 1;
    }
    
    if (!parse_double(num2_str, &num2)) {
        fprintf(stderr, "Invalid second number: %s\n", num2_str);
        return 1;
    }
    
    if (!is_valid_operator(op)) {
        fprintf(stderr, "Invalid operator: %c\n", op);
        return 1;
    }
    
    result = calculate_expression(num1, num2, op);
    
    if (isnan(result)) {
        fprintf(stderr, "Mathematical error in calculation\n");
        return 1;
    }
    
    if (!isfinite(result)) {
        fprintf(stderr, "Result is infinite\n");
        return 1;
    }
    
    printf("Result: %.6f\n", result);
    
    return 0;
}