//DeepSeek-V3 V2.5 Category: Safe ; Style: io_bound ; Variation: numeric_computation
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <stdint.h>
#include <limits.h>

#define MAX_INPUT_LEN 256

double calculate_expression(const char* input) {
    double num1, num2;
    char op;
    int parsed = sscanf(input, "%lf %c %lf", &num1, &op, &num2);
    if (parsed != 3) {
        return NAN;
    }
    
    switch (op) {
        case '+':
            return num1 + num2;
        case '-':
            return num1 - num2;
        case '*':
            return num1 * num2;
        case '/':
            if (fabs(num2) < 1e-12) {
                return NAN;
            }
            return num1 / num2;
        case '^':
            if (num1 < 0 && fmod(num2, 1.0) != 0) {
                return NAN;
            }
            return pow(num1, num2);
        default:
            return NAN;
    }
}

int main(void) {
    char input[MAX_INPUT_LEN + 1];
    int valid_count = 0;
    int total_count = 0;
    
    printf("Enter arithmetic expressions (number operator number):\n");
    printf("Supported operators: + - * / ^\n");
    printf("Enter 'quit' to exit\n");
    
    while (1) {
        printf("Expression %d: ", total_count + 1);
        
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
        
        if (len >= MAX_INPUT_LEN) {
            printf("Input too long. Maximum %d characters allowed.\n", MAX_INPUT_LEN);
            continue;
        }
        
        if (strlen(input) == 0) {
            printf("Empty input. Please enter an expression.\n");
            continue;
        }
        
        total_count++;
        
        double result = calculate_expression(input);
        
        if (isnan(result)) {
            printf("Invalid expression or operation: %s\n", input);
        } else {
            valid_count++;
            printf("Result: %.6f\n", result);
        }
    }
    
    printf("\nSummary: %d valid expressions out of %d total attempts\n", valid_count, total_count);
    
    return 0;
}