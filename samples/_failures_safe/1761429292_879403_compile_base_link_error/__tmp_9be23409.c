//DeepSeek-V3 V2.5 Category: Safe ; Style: io_bound ; Variation: numeric_computation
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <stdint.h>
#include <limits.h>

#define MAX_INPUT_LEN 256

double calculate_expression(const char* input) {
    double result = 0.0;
    double current = 0.0;
    char op = '+';
    int i = 0;
    int decimal_places = 0;
    int in_decimal = 0;
    
    while (input[i] != '\0') {
        if (input[i] >= '0' && input[i] <= '9') {
            double digit = input[i] - '0';
            if (in_decimal) {
                decimal_places++;
                current = current + digit / pow(10, decimal_places);
            } else {
                current = current * 10.0 + digit;
            }
        } else if (input[i] == '.') {
            if (in_decimal) return NAN;
            in_decimal = 1;
        } else if (input[i] == '+' || input[i] == '-' || input[i] == '*' || input[i] == '/') {
            if (op == '+') result += current;
            else if (op == '-') result -= current;
            else if (op == '*') result *= current;
            else if (op == '/') {
                if (fabs(current) < 1e-12) return NAN;
                result /= current;
            }
            
            op = input[i];
            current = 0.0;
            in_decimal = 0;
            decimal_places = 0;
        } else if (input[i] != ' ' && input[i] != '\t' && input[i] != '\n') {
            return NAN;
        }
        i++;
    }
    
    if (op == '+') result += current;
    else if (op == '-') result -= current;
    else if (op == '*') result *= current;
    else if (op == '/') {
        if (fabs(current) < 1e-12) return NAN;
        result /= current;
    }
    
    return result;
}

int is_valid_expression(const char* input) {
    int digit_count = 0;
    int op_count = 0;
    int last_was_op = 1;
    
    for (int i = 0; input[i] != '\0'; i++) {
        if (input[i] >= '0' && input[i] <= '9') {
            digit_count++;
            last_was_op = 0;
        } else if (input[i] == '+' || input[i] == '-' || input[i] == '*' || input[i] == '/') {
            if (last_was_op) return 0;
            op_count++;
            last_was_op = 1;
        } else if (input[i] == '.') {
            if (last_was_op) return 0;
        } else if (input[i] != ' ' && input[i] != '\t' && input[i] != '\n') {
            return 0;
        }
    }
    
    return digit_count >= 1 && op_count >= 1 && !last_was_op;
}

int main(void) {
    char input[MAX_INPUT_LEN + 1];
    int valid_inputs = 0;
    double total = 0.0;
    
    printf("Simple Calculator (Enter 'quit' to exit)\n");
    printf("Enter expressions like: 5 + 3.2 * 2\n");
    
    while (1) {
        printf("> ");
        
        if (fgets(input, sizeof(input), stdin) == NULL) {
            break;
        }
        
        input[strcspn(input, "\n")] = '\0';
        
        if (strcmp(input, "quit") == 0) {
            break;
        }
        
        if (strlen(input) == 0) {
            continue;
        }
        
        if (!is_valid_expression(input)) {
            printf("Error: Invalid expression format\n");
            continue;
        }
        
        double result = calculate_expression(input);
        
        if (isnan(result)) {
            printf("Error: Calculation error (division by zero or invalid input)\n");
            continue;
        }
        
        printf("Result: %.6f\n", result);
        valid_inputs++;
        total += result;
    }
    
    if (valid_inputs > 0) {
        printf("\nSummary: %d calculations, average result: %.6f\n", 
               valid_inputs, total / valid_inputs);
    } else {
        printf("No valid calculations performed.\n");
    }
    
    return 0;
}