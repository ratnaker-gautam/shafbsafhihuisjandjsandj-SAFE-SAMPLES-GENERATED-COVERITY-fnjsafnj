//DeepSeek-V3 V2.5 Category: Safe ; Style: io_bound ; Variation: numeric_computation
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <ctype.h>

#define MAX_INPUT_LEN 100

double calculate_expression(const char* expr, int* error) {
    double result = 0.0;
    double current = 0.0;
    char op = '+';
    int i = 0;
    int decimal_places = 0;
    int in_decimal = 0;
    
    *error = 0;
    
    while (expr[i] != '\0') {
        if (isspace((unsigned char)expr[i])) {
            i++;
            continue;
        }
        
        if (isdigit((unsigned char)expr[i])) {
            double num = 0.0;
            
            while (isdigit((unsigned char)expr[i]) || expr[i] == '.') {
                if (expr[i] == '.') {
                    if (in_decimal) {
                        *error = 1;
                        return 0.0;
                    }
                    in_decimal = 1;
                    decimal_places = 1;
                } else {
                    if (in_decimal) {
                        num = num + (expr[i] - '0') / pow(10.0, decimal_places);
                        decimal_places++;
                    } else {
                        num = num * 10.0 + (expr[i] - '0');
                    }
                }
                i++;
            }
            
            switch (op) {
                case '+': result += current; current = num; break;
                case '-': result += current; current = -num; break;
                case '*': current *= num; break;
                case '/': 
                    if (fabs(num) < 1e-12) {
                        *error = 1;
                        return 0.0;
                    }
                    current /= num; 
                    break;
                default: *error = 1; return 0.0;
            }
            
            in_decimal = 0;
            decimal_places = 0;
            continue;
        }
        
        if (expr[i] == '+' || expr[i] == '-' || expr[i] == '*' || expr[i] == '/') {
            op = expr[i];
            i++;
            continue;
        }
        
        *error = 1;
        return 0.0;
    }
    
    result += current;
    return result;
}

int main() {
    char input[MAX_INPUT_LEN + 1];
    int error;
    double result;
    
    printf("Enter a simple arithmetic expression (numbers and + - * /): ");
    
    if (fgets(input, sizeof(input), stdin) == NULL) {
        fprintf(stderr, "Error reading input\n");
        return 1;
    }
    
    size_t len = strlen(input);
    if (len > 0 && input[len - 1] == '\n') {
        input[len - 1] = '\0';
    }
    
    if (len == 1 && input[0] == '\0') {
        fprintf(stderr, "Empty input\n");
        return 1;
    }
    
    if (len >= MAX_INPUT_LEN) {
        fprintf(stderr, "Input too long\n");
        return 1;
    }
    
    result = calculate_expression(input, &error);
    
    if (error) {
        fprintf(stderr, "Invalid expression\n");
        return 1;
    }
    
    if (!isfinite(result)) {
        fprintf(stderr, "Result is not finite\n");
        return 1;
    }
    
    printf("Result: %.6f\n", result);
    
    return 0;
}