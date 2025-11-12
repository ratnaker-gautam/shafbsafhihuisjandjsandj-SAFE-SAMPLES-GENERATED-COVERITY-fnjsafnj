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
    double num1, num2, result;
    char op;
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
        
        input[strcspn(input, "\n")] = '\0';
        
        if (strcmp(input, "quit") == 0) {
            break;
        }
        
        valid_input = 0;
        
        if (sscanf(input, "%lf %c %lf", &num1, &op, &num2) == 3) {
            if (is_valid_operator(op)) {
                result = calculate_expression(num1, num2, op);
                if (!isnan(result)) {
                    printf("Result: %.6f\n", result);
                    valid_input = 1;
                } else {
                    printf("Error: Invalid operation\n");
                }
            }
        }
        
        if (!valid_input) {
            printf("Error: Invalid input format. Use: number operator number\n");
        }
    }
    
    printf("Goodbye!\n");
    return 0;
}