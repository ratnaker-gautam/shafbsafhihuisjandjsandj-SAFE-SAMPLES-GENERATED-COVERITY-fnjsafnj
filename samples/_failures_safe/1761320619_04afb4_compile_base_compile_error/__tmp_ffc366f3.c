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
            if ((b > 0 && a > DBL_MAX - b) || (b < 0 && a < -DBL_MAX - b)) {
                return NAN;
            }
            return a + b;
        case '-': 
            if ((b < 0 && a > DBL_MAX + b) || (b > 0 && a < -DBL_MAX + b)) {
                return NAN;
            }
            return a - b;
        case '*': 
            if (a != 0.0 && b != 0.0) {
                if (fabs(a) > DBL_MAX / fabs(b)) {
                    return NAN;
                }
            }
            return a * b;
        case '/': 
            if (fabs(b) < 1e-12) {
                return NAN;
            }
            if (a != 0.0 && fabs(a) > DBL_MAX * fabs(b)) {
                return NAN;
            }
            return a / b;
        case '^':
            if (a < 0 && fmod(b, 1.0) != 0) {
                return NAN;
            }
            if (a == 0.0 && b < 0) {
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
    char token1[MAX_INPUT_LEN], token2[MAX_INPUT_LEN];
    
    printf("Enter expression (num1 operator num2): ");
    if (fgets(input, sizeof(input), stdin) == NULL) {
        fprintf(stderr, "Error reading input\n");
        return 1;
    }
    
    size_t len = strlen(input);
    if (len > 0 && input[len - 1] == '\n') {
        input[len - 1] = '\0';
    }
    
    if (sscanf(input, "%255s %c %255s", token1, &op, token2) != 3) {
        fprintf(stderr, "Invalid input format. Use: number operator number\n");
        return 1;
    }
    
    if (!parse_double(token1, &num1)) {
        fprintf(stderr, "Invalid first number: %s\n", token1);
        return 1;
    }
    
    if (!parse_double(token2, &num2)) {
        fprintf(stderr, "Invalid second number: %s\n", token2);
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
    
    if (result > DBL_MAX || result < -DBL_MAX) {
        fprintf(stderr, "Result out of representable range\n");
        return 1;
    }
    
    printf("Result: %.6f\n", result);
    
    return 0;
}