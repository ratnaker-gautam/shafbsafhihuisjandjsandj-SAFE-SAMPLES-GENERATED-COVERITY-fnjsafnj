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
            if (b == 0.0) {
                fprintf(stderr, "Error: Division by zero\n");
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
    char op_char;
    int parsed = sscanf(input, "%lf %c %lf", a, &op_char, b);
    if (parsed != 3) {
        return 0;
    }
    if (!is_valid_operator(op_char)) {
        return 0;
    }
    *op = op_char;
    return 1;
}

int main(void) {
    char input[MAX_INPUT_LEN + 1];
    double num1, num2, result;
    char operation;
    int valid_input;
    
    printf("Numeric Expression Calculator\n");
    printf("Enter expressions in format: number operator number\n");
    printf("Supported operators: + - * / ^\n");
    printf("Type 'quit' to exit\n\n");
    
    while (1) {
        printf("> ");
        
        if (fgets(input, sizeof(input), stdin) == NULL) {
            break;
        }
        
        size_t len = strlen(input);
        if (len > 0 && input[len - 1] == '\n') {
            input[len - 1] = '\0';
        }
        
        if (strcmp(input, "quit") == 0) {
            break;
        }
        
        if (strlen(input) == 0) {
            continue;
        }
        
        valid_input = parse_input(input, &num1, &operation, &num2);
        if (!valid_input) {
            fprintf(stderr, "Error: Invalid input format. Use: number operator number\n");
            continue;
        }
        
        if (operation == '^' && num1 < 0 && fmod(num2, 1.0) != 0.0) {
            fprintf(stderr, "Error: Negative base with fractional exponent\n");
            continue;
        }
        
        result = calculate_expression(num1, num2, operation);
        
        if (isnan(result)) {
            fprintf(stderr, "Error: Calculation failed\n");
            continue;
        }
        
        if (isinf(result)) {
            fprintf(stderr, "Error: Result is infinite\n");
            continue;
        }
        
        printf("Result: %.6f\n", result);
    }
    
    printf("Goodbye!\n");
    return 0;
}