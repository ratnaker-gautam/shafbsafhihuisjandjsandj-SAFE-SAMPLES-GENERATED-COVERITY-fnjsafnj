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
        case '+': 
            if ((a > 0 && b > DBL_MAX - a) || (a < 0 && b < -DBL_MAX - a)) {
                return NAN;
            }
            return a + b;
        case '-': 
            if ((a > 0 && -b > DBL_MAX - a) || (a < 0 && -b < -DBL_MAX - a)) {
                return NAN;
            }
            return a - b;
        case '*': 
            if (a != 0.0 && fabs(b) > fabs(DBL_MAX / a)) {
                return NAN;
            }
            return a * b;
        case '/': 
            if (fabs(b) < 1e-12) {
                return NAN;
            }
            if (a != 0.0 && fabs(b) < 1.0 && fabs(a) > fabs(DBL_MAX * b)) {
                return NAN;
            }
            return a / b;
        case '^':
            if (a < 0 && fmod(b, 1.0) != 0.0) {
                return NAN;
            }
            if (a == 0.0 && b < 0.0) {
                return NAN;
            }
            double temp = pow(a, b);
            if (isinf(temp) || isnan(temp)) {
                return NAN;
            }
            return temp;
        default: return NAN;
    }
}

int is_valid_operator(char op) {
    return op == '+' || op == '-' || op == '*' || op == '/' || op == '^';
}

int parse_double(const char *str, double *result) {
    if (str == NULL || result == NULL) {
        return 0;
    }
    char *endptr;
    *result = strtod(str, &endptr);
    if (endptr == str || *endptr != '\0') {
        return 0;
    }
    if (*result == HUGE_VAL || *result == -HUGE_VAL || isnan(*result)) {
        return 0;
    }
    return 1;
}

int main(void) {
    char input[MAX_INPUT_LEN];
    char num1_str[MAX_INPUT_LEN], num2_str[MAX_INPUT_LEN];
    char op;
    double num1, num2, result;
    int parsed_count;
    
    printf("Simple Numeric Calculator\n");
    printf("Enter expression (e.g., 5.2 + 3.7): ");
    
    if (fgets(input, sizeof(input), stdin) == NULL) {
        fprintf(stderr, "Error reading input\n");
        return 1;
    }
    
    size_t len = strlen(input);
    if (len > 0 && input[len - 1] == '\n') {
        input[len - 1] = '\0';
    }
    
    if (strlen(input) >= MAX_INPUT_LEN - 1) {
        fprintf(stderr, "Input too long\n");
        return 1;
    }
    
    parsed_count = sscanf(input, "%255s %c %255s", num1_str, &op, num2_str);
    if (parsed_count != 3) {
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
    
    if (isinf(result) || result > DBL_MAX || result < -DBL_MAX) {
        fprintf(stderr, "Result out of representable range\n");
        return 1;
    }
    
    printf("Result: %.6f\n", result);
    
    return 0;
}