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
        char temp_input[MAX_INPUT_LEN];
        strncpy(temp_input, input, sizeof(temp_input) - 1);
        temp_input[sizeof(temp_input) - 1] = '\0';
        
        char *token1 = strtok(temp_input, " ");
        char *token2 = token1 ? strtok(NULL, " ") : NULL;
        char *token3 = token2 ? strtok(NULL, " ") : NULL;
        char *token4 = token3 ? strtok(NULL, " ") : NULL;
        
        if (token1 && token2 && token3 && !token4) {
            if (parse_double(token1, &num1) && 
                strlen(token2) == 1 && 
                is_valid_operator(token2[0]) && 
                parse_double(token3, &num2)) {
                
                op = token2[0];
                valid_input = 1;
            }
        }
        
        if (!valid_input) {
            printf("Invalid input format. Use: number operator number\n");
            continue;
        }
        
        result = calculate_expression(num1, num2, op);
        
        if (isnan(result)) {
            printf("Error: Invalid operation\n");
        } else if (isinf(result)) {
            printf("Error: Result too large\n");
        } else {
            printf("Result: %.6g\n", result);
        }
    }
    
    printf("Goodbye!\n");
    return 0;
}