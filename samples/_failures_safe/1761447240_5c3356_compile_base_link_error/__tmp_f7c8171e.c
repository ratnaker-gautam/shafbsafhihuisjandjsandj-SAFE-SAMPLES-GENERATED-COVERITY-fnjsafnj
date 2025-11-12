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
    
    printf("Enter first number: ");
    if (fgets(input, sizeof(input), stdin) == NULL) {
        fprintf(stderr, "Error reading input\n");
        return 1;
    }
    input[strcspn(input, "\n")] = '\0';
    
    if (!parse_double(input, &num1)) {
        fprintf(stderr, "Invalid number format\n");
        return 1;
    }
    
    printf("Enter operator (+, -, *, /, ^): ");
    if (fgets(input, sizeof(input), stdin) == NULL) {
        fprintf(stderr, "Error reading input\n");
        return 1;
    }
    if (sscanf(input, " %c", &op) != 1 || !is_valid_operator(op)) {
        fprintf(stderr, "Invalid operator\n");
        return 1;
    }
    
    printf("Enter second number: ");
    if (fgets(input, sizeof(input), stdin) == NULL) {
        fprintf(stderr, "Error reading input\n");
        return 1;
    }
    input[strcspn(input, "\n")] = '\0';
    
    if (!parse_double(input, &num2)) {
        fprintf(stderr, "Invalid number format\n");
        return 1;
    }
    
    result = calculate_expression(num1, num2, op);
    
    if (isnan(result)) {
        fprintf(stderr, "Mathematical error in calculation\n");
        return 1;
    }
    
    if (result == HUGE_VAL || result == -HUGE_VAL) {
        fprintf(stderr, "Result out of representable range\n");
        return 1;
    }
    
    printf("Result: %.6f\n", result);
    
    return 0;
}